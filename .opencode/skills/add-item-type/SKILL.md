---
name: add-item-type
description: Template for adding a new bazaar item type across backend and frontend
---

## Pipeline

### Backend
1. Define struct in `packets/rc_blist.h` (add to `ItemDataFields` variant)
2. Parse from game packet in `packets/rc_blist.cpp` (add factory case in `ItemDataFieldsFactory`)
3. Add JSON serialization in `rc_blist.cpp` (add `json()` visitor overload)

### Frontend
4. Define TypeScript interface in `types/search.ts`
5. Add type guard function (e.g., `isMyNewType(data)`)
6. Add rendering block in `components/ItemDetailWindow.svelte`
7. Register in `SearchResultData` union type

### Reference
- Existing patterns: `ShellData`, `AccessoryData`, `SpecialistData`
- Backend always sends `shells` array for weapons/armor with `ShellEffect {grade, vnum, value, upgrade}`
