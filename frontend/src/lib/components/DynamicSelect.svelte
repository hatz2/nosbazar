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

<select bind:value class="bazar-select">
	{#each options as option (option.value)}
		<option value={option.value}>
			{option.label}
		</option>
	{/each}
</select>
