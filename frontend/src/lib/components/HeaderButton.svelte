<script lang="ts">
	type Props = {
		href: string;
		label: string;
		iconPath: string;
	};

	let { href, label, iconPath }: Props = $props();
</script>

<a
	class="header-button"
	{href}
	title={label}
	aria-label={label}
	target="_blank"
	rel="external noopener noreferrer"
>
	<svg viewBox="0 0 24 24" fill="currentColor" aria-hidden="true">
		<path d={iconPath} />
	</svg>
</a>

<style>
	.header-button {
		display: inline-flex;
		align-items: center;
		justify-content: center;
		width: 2rem;
		height: 2rem;
		padding: 0;
		color: #c9d1d9;
		background: linear-gradient(#2f2f2f, #1d1d1d);
		border: 1px solid #606467;
		border-radius: 0.25rem;
		text-decoration: none;
	}

	.header-button:hover {
		color: #f6b626;
		border-color: #f6b626;
	}

	.header-button svg {
		width: 1.5rem;
		height: 1.5rem;
	}
</style>
