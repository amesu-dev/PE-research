# Convention of code style
## Base style

- **snake_case** for `local variables and consts, `
- **SCREAMING_SNAKE_CASE** for `structs, global consts, enum values`

- **Devide logical sections with:**

```
// ---------------------
// <Section name>

// ---------------------
```

## Prefix convention
- `p_` - pointer to some structure in memory

- `rra_` - `raw relative address` - points to some struct in file represintation (relative to something) 
- `rva_` - `raw virtual address` - in .exe memory space (not our, current)

- `va_` - `virtual address` - in our memory (butch)
- `ra_` - `raw address` - absolute offset from file begining