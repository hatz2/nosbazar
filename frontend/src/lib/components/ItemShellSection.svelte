<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import {
		ShellGradeColor,
		formatAppliedShellEffectString,
		formatShellOptionString
	} from '$lib/utils/format';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { isShellData } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

{#if 'shells' in item.data && item.data.shells.length > 0}
	<p>
		{#each item.data.shells as shell, i (i)}
			<span style="color: {ShellGradeColor[shell.grade] ?? '#FFFFFF'}">
				{formatAppliedShellEffectString(shell)}
			</span><br />
		{/each}
	</p>
{/if}

{#if isShellData(item.data)}
	{#if item.data.shell_options.length > 0}
		<p>
			{#each item.data.shell_options as option, i (i)}
				<span style="color: {ShellGradeColor[option.grade] ?? '#FFFFFF'}">
					{formatShellOptionString(option)}
				</span><br />
			{/each}
		</p>
	{:else}
		<p class="red">
			{get_const_string(ConstStringKey.OptionNotIdentified)}
		</p>
	{/if}
{/if}

<style>
	.red {
		color: #ff323d;
	}
</style>
