<script lang="ts">
	import HeaderButton from '$lib/components/HeaderButton.svelte';
	import LanguageSelector from '$lib/components/LanguageSelector.svelte';
	import ServerSelector from '$lib/components/ServerSelector.svelte';
	import { DISCORD_ICON_PATH, GITHUB_ICON_PATH, GITHUB_LINK, KOFI_ICON_PATH } from '$lib/constants';
	import favicon from '$lib/assets/favicon.png';

	const links = [
		{
			name: 'GitHub',
			href: GITHUB_LINK,
			iconPath: GITHUB_ICON_PATH
		}
		// {
		// 	name: 'Discord',
		// 	href: '#',
		// 	iconPath: DISCORD_ICON_PATH
		// },
		// {
		// 	name: 'Donate',
		// 	href: '#',
		// 	iconPath: KOFI_ICON_PATH
		// }
	];

	let { server = $bindable() } = $props<{
		server?: number;
	}>();
</script>

<header class="top-bar">
	<div class="top-row">
		<div class="brand-row">
			<img class="logo" src={favicon} alt="Logo icon" />
			<div class="brand">
				<h1 class="app-title">NosBazar</h1>
				<p class="app-subtitle">Online bazar search for the MMORPG NosTale</p>
			</div>
		</div>

		<div class="spacer"></div>
		<nav class="links">
			{#each links as link (link.name)}
				<HeaderButton href={link.href} label={link.name} iconPath={link.iconPath} />
			{/each}
		</nav>
		<ServerSelector bind:value={server} />
		<LanguageSelector />
	</div>
</header>

<style>
	.top-bar {
		display: flex;
		flex-direction: column;
		gap: 0.5rem;
		padding: 1rem;
		padding-bottom: 4rem;
		background: linear-gradient(
			rgba(47, 47, 47, 1) 0%,
			rgba(47, 47, 47, 0.8) 50%,
			rgba(47, 47, 47, 0.8) 50%,
			rgba(27, 27, 27, 0) 100%
		);
	}

	.top-row {
		display: flex;
		align-items: center;
		gap: 0.75rem;
		width: 100%;
	}

	.brand-row {
		display: flex;
		align-items: center;
		gap: 0.75rem;
	}

	.logo {
		flex-shrink: 0;
		align-self: center;
		width: auto;
		height: auto;
		max-width: 100%;
		object-fit: contain;
	}

	.brand {
		display: flex;
		flex-direction: column;
		gap: 0.15rem;
	}

	.app-title {
		margin: 0;
		font-size: 1.5rem;
		font-weight: bold;
		color: #ffffff;
		text-shadow:
			2px 0 #0d0d0d,
			-2px 0 #0d0d0d,
			0 2px #0d0d0d,
			0 -2px #0d0d0d,
			1px 1px #0d0d0d,
			-1px -1px #0d0d0d,
			1px -1px #0d0d0d,
			-1px 1px #0d0d0d;
	}

	.app-subtitle {
		margin: 0;
		font-size: 0.9rem;
		color: #9a9a9a;
	}

	.spacer {
		flex: 1;
	}

	.links {
		display: flex;
		align-items: center;
		gap: 0.5rem;
	}
</style>
