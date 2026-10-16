---
name: add-item-type
description: Template for adding a new bazaar item type across backend and frontend
---

## Pipeline

Follow the repository's `AGENTS.md`. Paths below are relative to the repository root.

### Backend
1. Define struct in `backend/src/packets/rc_blist.h` (add to `ItemDataFields` variant)
2. Parse from game packet in `backend/src/packets/rc_blist.cpp` (add factory case in `ItemDataFieldsFactory`)
3. Add JSON serialization in `backend/src/packets/rc_blist.cpp` (use the existing `std::visit` dispatch)

### Frontend
4. Define TypeScript interface in `frontend/src/lib/types/search.ts`
5. Add type guard function (e.g., `isMyNewType(data)`)
6. Add rendering block in `frontend/src/lib/components/ItemDetailWindow.svelte`
7. Register in `SearchResultData` union type

### Reference
- Existing patterns: `ShellData`, `AccessoryData`, `SpecialistData`
- Backend always sends `shells` array for weapons/armor with `ShellEffect {grade, vnum, value, upgrade}`
