# AmethystOS x86 Memory Management

**WARNING**: The memory is still not done. All of these informations may change during the next weeks!

## How the physical memory is segmented

| P-MEM Start | P-MEM End   | Usage              |
| :---------- | :---------- | :----------------- |
| `0000.7E00` | `0000.9FFF` | Bootloader Code    |
| `0000.A000` | `0000.FFFF` | Kernel Code 16bits |
| `0001.0000` | `000F.FFFF` | Kernel Code 32bits |
| `0010.0000` | `0FFF.FFFF` | Application space  |
