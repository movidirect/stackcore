# 🧪 Guía de Testing - Nuevas Funcionalidades Stackcore 3D

## 🎯 Funcionalidades a Probar

### 1. Sistema de Niveles Progresivos
**¿Qué probar?**
- La velocidad de caída aumenta gradualmente
- Los niveles suben cada 10 líneas completadas
- Se muestra información del nivel en la interfaz
- Bonificaciones de puntos por nivel

**Cómo probar:**
1. Inicia el juego
2. Verifica que aparece "Level: 1" y "Lines: 0/10" en la interfaz
3. Completa líneas destruyendo niveles completos (9x9)
4. Observa si el contador de líneas aumenta
5. Al completar 10 líneas, verifica:
   - El nivel sube a 2
   - El contador se resetea a "Lines: 0/20"
   - La velocidad de caída aumenta notablemente
   - Se reproduce un sonido de level up

### 2. Sistema de Ghost Block (Sombra)
**¿Qué probar?**
- Se muestra una "sombra" wireframe del bloque actual
- La sombra indica dónde caerá el bloque
- Se puede activar/desactivar con la tecla H

**Cómo probar:**
1. Observa el bloque actual cayendo
2. Debe aparecer una versión wireframe (líneas) mostrando dónde aterrizará
3. Mueve el bloque (←→↑↓) y verifica que la sombra se actualiza
4. Rota el bloque (WASD, QE) y verifica que la sombra rota también
5. Presiona `H` para desactivar el ghost block
6. Presiona `H` de nuevo para reactivarlo

### 3. Sistema de Preview de Bloques
**¿Qué probar?**
- Los bloques siguen un patrón determinístico
- No se repiten inmediatamente los mismos tipos

**Cómo probar:**
1. Observa el tipo de bloque actual
2. Déjalo caer y observa el siguiente
3. Verifica que hay variedad en los tipos de bloques
4. Los bloques deben seguir la secuencia generada por el sistema de preview

### 4. Mejoras de Audio y Feedback
**¿Qué probar?**
- Sonidos diferenciados para diferentes acciones
- Level up tiene sonido específico
- Line clear tiene sonido específico

**Cómo probar:**
1. Asegúrate de que el sonido esté activado (M para toggle)
2. Presiona teclas de movimiento → escucha sonido de tecla
3. Deja caer un bloque → escucha sonido de aterrizaje  
4. Completa una línea → escucha sonido de line clear
5. Sube de nivel → escucha sonido de level up
6. Game over → escucha sonido específico

### 5. Sistema de Configuración y Ayuda
**¿Qué probar?**
- La nueva ventana de ayuda se abre con F1.
- El modo demo se activa con TAB.
- El modo demo se puede iniciar desde la línea de comandos.

**Cómo probar:**
1. Presiona `F1`: Debe aparecer una ventana de ImGui con la lista de controles.
2. Presiona `TAB`: El bot de IA debe tomar el control del juego (Modo Demo).
3. Cierra el juego y ejecútalo desde la terminal con `./Output/stackcore --demo`: El juego debe iniciar directamente con el bot activo.
4. Verifica que al activar el modo Demo, el "Ghost Block" NO se activa automáticamente si estaba apagado.

## 🎮 Controles para Testing

```
MOVIMIENTO Y GAMEPLAY:
↑↓←→      : Mover bloque
ESPACIO   : Drop rápido  
WASD      : Rotación X,Y
QE        : Rotación Z

SISTEMA Y AYUDA:
TAB       : Toggle Modo Demo (Bot AI) ⭐ CAMBIADO
F1        : Ventana de Ayuda ⭐ NUEVO
H         : Toggle Ghost Block
N         : Toggle Next Preview
M         : Toggle Sound
P         : Pause
R/ENTER   : Reset Juego

CÁMARA:
CTRL+WASD : Control de cámara
ESC       : Salir
```

## 📊 Checklist de Testing

### ✅ Sistema de Ayuda y Demo
- [ ] Ventana de ayuda (F1) se muestra y oculta correctamente.
- [ ] La tecla TAB activa/desactiva el bot de IA.
- [ ] El parámetro `--demo` inicia el juego en modo automático.
- [ ] El Ghost Block permanece desactivado al entrar en modo demo (si estaba apagado).

### ✅ Configuración
- [ ] Interfaz actualizada con nuevos controles (TAB/F1).
- [ ] Todos los toggles funcionan correctamente.
- [ ] No hay logs `INFO:` innecesarios en la consola al iniciar.

### ✅ Instalador (Opcional en Linux)
- [ ] Ejecutar `makensis installer.nsi` genera el archivo `StackCoreSetup.exe`.

## 🐛 Problemas Conocidos a Reportar

Si encuentras alguno de estos problemas, repórtalo:

1. **Performance**: ¿El ghost block causa lag?
2. **Audio**: ¿Se superponen sonidos de manera molesta?
3. **Visual**: ¿El ghost block confunde visualmente?
4. **Gameplay**: ¿Los niveles progresan demasiado rápido/lento?
5. **Interfaz**: ¿Información del nivel es clara?

## 🎯 Objetivos de Testing

1. **Funcionalidad**: Todo debe funcionar como se describe
2. **Usabilidad**: Las mejoras deben hacer el juego más divertido
3. **Performance**: No debe haber degradación significativa del rendimiento
4. **Estabilidad**: El juego no debe crashear con las nuevas funcionalidades

¡Disfruta probando las nuevas funcionalidades! 🎮✨ 