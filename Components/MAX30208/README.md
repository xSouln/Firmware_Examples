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
- Temperature sensor
- The component includes:
  - abstract level for managing low-level functions
  - status monitoring and management
  - settings management
- Files:
  - [MAX30208_Types.h](MAX30208_Types.h) - contains defining types
  - [MAX30208_Config.h](MAX30208_Config.h) - contains default settings, constants, and other configurable values
  - [MAX30208_Adapter.c](MAX30208_Adapter.c) - contains a low-level adapter for hardware management functions
  - [MAX30208_Control.c](MAX30208_Control.c) - contains management functions, status updates, processing

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
- [Libs/MAX30208](/Libs/MAX30208)
- [Core/Inc/main.h](/Core/Inc/main.h)
- [Core/Inc/i2c.h](/Core/Inc/i2c.h)