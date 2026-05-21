<script lang="ts" generics="T">
	type SelectOption<T> = {
		label: string;
		value: T;
	};

	type Props = {
		options?: SelectOption<T>[];
		value?: T;
	};

	let { options = [], value = $bindable() }: Props = $props();
</script>

<select bind:value>
	{#each options as option (option.value)}
		<option value={option.value}>
			{option.label}
		</option>
	{/each}
</select>

<style>
</style>
