import { get_const_string } from '$lib/services/constStringService.svelte';
import {
	ArmorShellOptionsConstStrings,
	CellonOptionsConstStrings,
	ConstStringKey,
	WeaponShellOptionsConstStrings,
	ShellQualifierKeys
} from '$lib/types/constStringKeys';
import type { CellonOption, ShellOptionData } from '$lib/types/search';

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

export const ShellGradeColor: Record<number, string> = {
	1: '#FFCC00',
	2: '#FFFF00',
	3: '#99FF66',
	4: '#CCFF99',
	5: '#CC9AFF',
	6: '#CC9AFF',
	7: '#CC9AFF',
	8: '#CC9AFF',
	9: '#FF7AB2',
	10: '#FF7AB2',
	11: '#FF7AB2',
	12: '#FF7AB2',
	13: '#FFCC00',
	14: '#FFFF00',
	15: '#99FF66',
	16: '#CCFF99',
	17: '#CC9AFF',
	18: '#CC9AFF',
	19: '#CC9AFF',
	20: '#CC9AFF',
	21: '#FF7AB2',
	22: '#FF7AB2',
	23: '#FF7AB2',
	24: '#FF7AB2',
};

const ShellGradeLabel: Record<number, string> = {
	1: 'C',
	2: 'B',
	3: 'A',
	4: 'S',
	5: 'C',
	6: 'B',
	7: 'A',
	8: 'S',
	9: 'C',
	10: 'B',
	11: 'A',
	12: 'S',
	13: 'C',
	14: 'B',
	15: 'A',
	16: 'S',
	17: 'C',
	18: 'B',
	19: 'A',
	20: 'S',
	21: 'C',
	22: 'B',
	23: 'A',
	24: 'S',
};

export function formatShellOptionString(shellOption: ShellOptionData): string {
	const isArmor = shellOption.grade > 12;
	const constStrKey = isArmor
		? ArmorShellOptionsConstStrings[shellOption.vnum]
		: WeaponShellOptionsConstStrings[shellOption.vnum];
	if (constStrKey === undefined) {
		return '';
	}
	let template = get_const_string(constStrKey);
	if (template.includes('%s')) {
		template = formatString(template, shellOption.value);
	}
	let result = '';
	const qualifierKey = isArmor ? undefined : ShellQualifierKeys[shellOption.vnum];
	if (qualifierKey !== undefined) {
		result += get_const_string(qualifierKey) + ' ';
	}
	const gradeLabel = ShellGradeLabel[shellOption.grade];
	if (gradeLabel) {
		result += gradeLabel + '-';
	}
	result += template;
	return result;
}
