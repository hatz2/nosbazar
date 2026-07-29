<script lang="ts">
	import type { EnrichedSearchResult } from '$lib/types/search';
	import { lang } from '$lib/stores/lang.svelte';
	import { ConstStringKey } from '$lib/types/constStringKeys';
	import { get_const_string } from '$lib/services/constStringService.svelte';
	import { isMountBeadItem } from '$lib/types/search';

	type Props = {
		item: EnrichedSearchResult;
	};

	let { item }: Props = $props();
</script>

{#if isMountBeadItem(item.data)}
	<p class="light-orange">
		{#if item.data.has_mount_inside}
			{item.contained_mount_item_static_data?.name[lang.current]}
			{get_const_string(ConstStringKey.InCustody)}<br />
		{:else}
			{get_const_string(ConstStringKey.NotUsed)}<br />
		{/if}
	</p>
{/if}

<style>
	.light-orange {
		color: #f5c478;
	}
</style>
