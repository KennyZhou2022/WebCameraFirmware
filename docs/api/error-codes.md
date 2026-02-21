# Error Code Table (Phase 0 Baseline)

## Rule
- `0`: success
- `<0`: failure

## Common Error Codes
| Code | Name | Meaning |
|---|---|---|
| 0 | ERR_OK | Success |
| -1 | ERR_INVALID_ARG | Invalid function argument |
| -2 | ERR_NOT_READY | Service/module not initialized |
| -3 | ERR_TIMEOUT | Operation timed out |
| -4 | ERR_IO | Generic I/O failure |
| -5 | ERR_NO_MEMORY | Memory allocation/pool exhausted |
| -6 | ERR_BUSY | Resource busy |
| -7 | ERR_NOT_FOUND | Requested item not found |
| -8 | ERR_STATE | Illegal state transition |
| -9 | ERR_NETWORK | Network operation failed |
| -10 | ERR_AUTH | Authentication/authorization failed |
| -11 | ERR_STORAGE | Persistent storage failure |
| -12 | ERR_INTEGRITY | Checksum/signature/integrity check failed |

## Module-Specific Ranges (Reserved)
| Range | Module Group |
|---|---|
| -100 to -149 | Camera/Media |
| -150 to -199 | Live Streaming |
| -200 to -249 | NAS Upload |
| -250 to -299 | Storage/Queue |
| -300 to -349 | Config/Provisioning |

## Notes
1. Keep this table backward-compatible once external interfaces depend on it.
2. Add new codes by extending reserved ranges first.
