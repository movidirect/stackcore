// Stackcore
// Copyright (C) 2023-2025 Jose R Arenas

#include "BotAI.h"
#include "Game.h"
#include "Utils.h"
#include <cstdlib>

#include <algorithm>

BotAI::BotAI(Game* g) : game(g) {
    reset();
}

void BotAI::reset() {
    botTimer = 0;
    aiTargetCalculated = false;
    botCommands = {false, false, false, false, false, false, false, false, false, false, false};
    currentRotX = 0;
    currentRotY = 0;
    currentRotZ = 0;
}

float BotAI::evaluateBoardState(const std::vector<Cube>& droppedCubes) {
    // OPTIMIZACIÓN: Función Lambda para consultar estado sin copiar la memoria
    auto isOccupied = [&](int x, int y, int z) {
        // Verificar primero si es uno de los cubos recién caídos (súper rápido)
        for (const auto& c : droppedCubes) {
            if (static_cast<int>(Utils::round(c.x)) == x &&
                static_cast<int>(Utils::round(c.y)) == y &&
                static_cast<int>(Utils::round(c.z)) == z) {
                return true;
            }
        }
        // Si no, buscar en el tablero original
        return game->board->getOccupiedPositions().count(std::make_tuple(x, y, z)) > 0;
        };
    float score = 0.0f;
    int maxZ = -5;
    int holes = 0;
    int bumpiness = 0;
    int blocksInBottomLevels = 0;
    
    // Perfil de alturas de cada columna
    int heights[9][9] = {0};
    
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            heights[x][y] = -5;
        }
    }

    // Analizar columnas para encontrar huecos y alturas
    for (int x = -4; x <= 4; ++x) {
        for (int y = -4; y <= 4; ++y) {
            bool blockFound = false;
            int colHeight = -5;
            
            for (int z = 4; z >= -4; --z) {
                if (isOccupied(x, y, z)) {
                    if (!blockFound) {
                        blockFound = true;
                        colHeight = z;
                        if (z > maxZ) maxZ = z; 
                    }
                } else {
                    if (blockFound) {
                        holes += 1; 
                        holes += (colHeight - z); 
                    }
                }
            }
            heights[x+4][y+4] = colHeight;
        }
    }
    
    // Analizar Bumpiness
    for (int x = 0; x < 9; ++x) {
        for (int y = 0; y < 9; ++y) {
            if (x < 8) bumpiness += std::abs(heights[x][y] - heights[x+1][y]);
            if (y < 8) bumpiness += std::abs(heights[x][y] - heights[x][y+1]);
        }
    }
    
    // Evaluar qué tan profundas cayeron las piezas de este turno
    float piecesDepthScore = 0.0f;
    for (const auto& cube : droppedCubes) {
        float z = Utils::round(cube.z);
        piecesDepthScore += (5.0f - z) * 500.0f; 
    }

    // Evaluar niveles completados o rellenados en el fondo
    int fullLinesBonus = 0;
    for (int z = -4; z <= maxZ; ++z) {
        int blocksInLevel = 0;
        for (int x = -4; x <= 4; ++x) {
            for (int y = -4; y <= 4; ++y) {
                if (isOccupied(x, y, z)) {
                    blocksInLevel++;
                }
            }
        }
        
        if (z == -4) blocksInBottomLevels += blocksInLevel;
        if (z == -3) blocksInBottomLevels += (blocksInLevel / 2); 
        
        if (blocksInLevel == 81) {
            fullLinesBonus += 10000; 
        } else {
            fullLinesBonus += (blocksInLevel * blocksInLevel); 
        }
    }

    float heightWeight    = -500.0f; 
    float holesWeight     = -50000.0f; // Castigo MASIVO por cada hueco tapado. Nunca valdrá la pena por una línea.
    float bumpinessWeight = -50.0f;  
    float bottomFillWeight= 200.0f;  
    
    score += (maxZ + 4) * heightWeight;
    score += holes * holesWeight;
    score += bumpiness * bumpinessWeight;
    score += fullLinesBonus;
    score += piecesDepthScore;
    score += blocksInBottomLevels * bottomFillWeight;

    return score;
}

void BotAI::calculateBestMove() {
    float bestScore = -999999.0f;
    aiTargetX = 0.0f;
    aiTargetY = 0.0f;
    aiTargetRotX = 0;
    aiTargetRotY = 0;
    aiTargetRotZ = 0;

    // OPTIMIZACIÓN: Cache de geometrías para evitar simular rotaciones redundantes
    std::set<std::vector<std::tuple<int, int, int>>> seenGeometries;

    for (int rX = 0; rX < 4; ++rX) {
        for (int rY = 0; rY < 4; ++rY) {
            for (int rZ = 0; rZ < 4; ++rZ) {
                
                Block testBlock(0.0f, 0.0f, game->initZ, 0); 
                testBlock.cubes = game->block->cubes;
                
                // Aplicar rotaciones asegurando que el motor físico real lo permita
                bool rotationPossible = true;
                for(int i=0; i<rX; i++) { if (!testBlock.tryRotateX(game->SCENE_LIMIT)) rotationPossible = false; }
                for(int i=0; i<rY; i++) { if (!testBlock.tryRotateY(game->SCENE_LIMIT)) rotationPossible = false; }
                for(int i=0; i<rZ; i++) { if (!testBlock.tryRotateZ(game->SCENE_LIMIT)) rotationPossible = false; }
                
                // Si la combinación de giros no es físicamente posible en el punto de spawn, descartarla
                if (!rotationPossible) continue;
                
                if (!testBlock.isWithinBounds(game->SCENE_LIMIT)) continue;
                
                // Generar la firma única (geometría) de esta rotación
                std::vector<std::tuple<int, int, int>> geometrySignature;
                for (const auto& c : testBlock.cubes) {
                    geometrySignature.push_back(std::make_tuple(
                        static_cast<int>(Utils::round(c.x)),
                        static_cast<int>(Utils::round(c.y)),
                        static_cast<int>(Utils::round(c.z))
                    ));
                }
                std::sort(geometrySignature.begin(), geometrySignature.end());
                
                // Si esta forma exacta ya la probamos, saltarla
                if (seenGeometries.find(geometrySignature) != seenGeometries.end()) {
                    continue; 
                }
                seenGeometries.insert(geometrySignature);
                
                for (float dx = -4.0f; dx <= 4.0f; dx += 1.0f) {
                    for (float dy = -4.0f; dy <= 4.0f; dy += 1.0f) {
                        Block transBlock = testBlock;
                        transBlock.move(dx, dy, 0.0f);
                        
                        // Verificar colisiones horizontales iniciales
                        if (!transBlock.isWithinBounds(game->SCENE_LIMIT) || game->board->checkCollision(&transBlock, 0.0f)) {
                            continue;
                        }
                        
                        // Simular caída libre (Ghost block logic)
                        while (true) {
                            if (game->checkCollisionWithWalls(&transBlock, game->MANUAL_DROP_SPEED) ||
                                game->board->checkCollision(&transBlock, game->MANUAL_DROP_SPEED)) {
                                break;
                            }
                            transBlock.move(0.0f, 0.0f, game->MANUAL_DROP_SPEED);
                        }
                        
                        // Evaluar el tablero resultante
                        float score = evaluateBoardState(transBlock.cubes);
                        
                        // Desempate: preferir movimientos que lleven el bloque más al fondo
                        float minDroppedZ = 4.0f;
                        for(const auto& c : transBlock.cubes) {
                            if(c.z < minDroppedZ) minDroppedZ = c.z;
                        }
                        score -= minDroppedZ * 0.5f; 

                        // Guardar la mejor jugada
                        if (score > bestScore) {
                            bestScore = score;
                            aiTargetX = dx;
                            aiTargetY = dy;
                            aiTargetRotX = rX;
                            aiTargetRotY = rY;
                            aiTargetRotZ = rZ;
                        }
                    }
                }
            }
        }
    }
    
    aiTargetCalculated = true;
    currentRotX = 0;
    currentRotY = 0;
    currentRotZ = 0;
}

void BotAI::update()
{
    // Limpiar comandos anteriores
    botCommands = {false, false, false, false, false, false, false, false, false, false, false};
    
    if (game->gameIsOver || game->gameIsPaused || !game->block) return;

    if (!aiTargetCalculated) {
        calculateBestMove();
        return; // Esperar un frame tras calcular
    }

    botTimer++;
    
    // Actuar cada N frames para que se vea la animación de la IA jugando
    // Ajustado a 15 para un equilibrio entre velocidad y observación clara (~4 acciones/seg)
    if (botTimer % 15 != 0) return; 

    // 1. Ejecutar Rotaciones primero
    if (currentRotX != aiTargetRotX) {
        botCommands.rotX = true;
        currentRotX = (currentRotX + 1) % 4;
        return;
    }
    if (currentRotY != aiTargetRotY) {
        botCommands.rotY = true;
        currentRotY = (currentRotY + 1) % 4;
        return;
    }
    if (currentRotZ != aiTargetRotZ) {
        botCommands.rotZ = true;
        currentRotZ = (currentRotZ + 1) % 4;
        return;
    }

    // 2. Ejecutar Traslaciones (X e Y)
    // Se compara con tolerancia de punto flotante
    if (game->currentBlockX < aiTargetX - 0.1f) {
        botCommands.right = true;
        return;
    } else if (game->currentBlockX > aiTargetX + 0.1f) {
        botCommands.left = true;
        return;
    }

    if (game->currentBlockY < aiTargetY - 0.1f) {
        botCommands.up = true;
        return;
    } else if (game->currentBlockY > aiTargetY + 0.1f) {
        botCommands.down = true;
        return;
    }

    // 3. Drop (Soltar cuando ya está alineado)
    botCommands.drop = true;
}
