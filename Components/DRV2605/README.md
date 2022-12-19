# Notes
___
## Content
- [Notes](#notes)
  - [Content](#content)
    - [Description](#description)
    - [Functional diagrams](#functional-diagrams)
    - [Folders](#folders)
    - [External dependencies](#external-dependencies)

### Description
- Haptic motor controller
- The component includes:
  - abstract level for managing low-level functions
  - status monitoring and management
  - settings management
- Files:
  - [DRV2605_Types.h](DRV2605_Types.h) - contains defining types
  - [DRV2605_Config.h](DRV2605_Config.h) - contains default settings, constants, and other configurable values
  - [DRV2605_Adapter.c](DRV2605_Adapter.c) - contains a low-level adapter for hardware management functions
  - [DRV2605_Control.c](DRV2605_Control.c) - contains management functions, status updates, processing

### Functional diagrams
- Initialization

  ![Initialization](Documents/functional_diagram-Initialization.jpg)

- Time synchronization

  ![Time synchronization](Documents/functional_diagram-TimeSynchronization.jpg)

- Handler

  ![Handler](Documents/functional_diagram-Handler.jpg)

### Folders
- [Documents](Documents) - contains a diagrams, description of the component modules, etc.

### External dependencies
- [Libs/DRV2605](/Libs/DRV2605)
- [Core/Inc/main.h](/Core/Inc/main.h)
- [Core/Inc/i2c.h](/Core/Inc/i2c.h)