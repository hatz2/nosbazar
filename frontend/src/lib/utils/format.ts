import { get_const_string } from '$lib/services/constStringService.svelte';
import { CellonOptionsConstStrings, ConstStringKey } from '$lib/types/constStringKeys';
import type { CellonOption } from '$lib/types/search';

export function formatString(template: string, ...args: (string | number)[]): string {
	const clean = template.replace(/<NEW_TYPE><\d+(,\d+)*>/g, '');
	let argIndex = 0;
	let result = '';
	for (let i = 0; i < clean.length; i++) {
		if (clean[i] === '%' && i + 1 < clean.length) {
			if (clean[i + 1] === 's' || clean[i + 1] === 'd') {
				result += String(args[argIndex++] ?? '%s');
				i++;
			} else if (clean[i + 1] === '%') {
				result += '%';
				i++;
			} else {
				result += clean[i];
			}
		} else {
			result += clean[i];
		}
	}
	return result;
}

export function formatCellonOptionString(cellonOption: CellonOption): string {
	const template = `${cellonOption.level}${get_const_string(ConstStringKey.Lv)} ${get_const_string(CellonOptionsConstStrings[cellonOption.vnum])}`;
	if (template.includes('%s')) {
		return formatString(template, cellonOption.value);
	}
	return `${template} ${cellonOption.value}`;
}
