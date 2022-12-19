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
- Power controller
- The component includes
  - managing abstract level for low-level functions
  - status monitoring and management
  - settings management
- Files:
  - [ADP5360_Types.h](ADP5360_Types.h) - contains defining types
  - [ADP5360_Config.h](ADP5360_Config.h) - contains default settings, constants, and other configurable values
  - [ADP5360_Adapter.c](ADP5360_Adapter.c) - contains a low-level adapter for hardware management functions
  - [ADP5360_Control.c](ADP5360_Control.c) - contains management functions, status updates, processing

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
- [Libs/ADP5360](/Libs/ADP5360)
- [Core/Inc/main.h](/Core/Inc/main.h)
- [Core/Inc/i2c.h](/Core/Inc/i2c.h)
