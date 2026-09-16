<script lang="ts">
	import { onMount } from 'svelte';
	import HeaderButton from '$lib/components/HeaderButton.svelte';
	import { GITHUB_ICON_PATH, API_BASE, GITHUB_LINK } from '$lib/constants';

	const version = '0.0.1';

	let serverOnline = $state<boolean | null>(null);

	onMount(async () => {
		try {
			const res = await fetch(`${API_BASE}/servers`);
			serverOnline = res.ok;
		} catch {
			serverOnline = false;
		}
	});
</script>

<footer class="bottom-bar">
	<p class="legal">
		© NosBazar contributors. All game content, images and trademarks belong to Gameforge 4D GmbH and
		Entwell Co., Ltd.
	</p>
	<p class="legal">
		This is an unofficial, non-commercial fan project and is not affiliated with or endorsed by
		Gameforge or Entwell.
	</p>
	<div class="footer-row">
		<span
			class="server-status"
			class:online={serverOnline === true}
			class:offline={serverOnline === false}
		>
			<span class="dot"></span>
			{serverOnline === null
				? 'Checking server...'
				: serverOnline
					? 'Server online'
					: 'Server offline'}
		</span>
		<HeaderButton href={GITHUB_LINK} label="GitHub" iconPath={GITHUB_ICON_PATH} />
		<a
			class="footer-link"
			href="https://nostale.gameforge.com"
			target="_blank"
			rel="external noopener noreferrer"
		>
			Official NosTale website
		</a>
		<span class="version">Version {version}</span>
	</div>
	<p class="privacy">No cookies are used and no tracking is performed.</p>
</footer>

<style>
	.bottom-bar {
		display: flex;
		flex-direction: column;
		align-items: center;
		gap: 0.5rem;
		margin-top: auto;
		padding: 7rem 1rem 1.5rem;
		background: linear-gradient(
			rgba(27, 27, 27, 0) 0%,
			rgba(47, 47, 47, 0.8) 50%,
			rgba(47, 47, 47, 0.8) 50%,
			rgba(47, 47, 47, 1) 100%
		);
	}

	.legal,
	.privacy {
		margin: 0;
		font-size: 0.875rem;
		color: #9a9a9a;
	}

	.footer-row {
		display: flex;
		align-items: center;
		gap: 0.75rem;
	}

	.server-status {
		display: inline-flex;
		align-items: center;
		gap: 0.4rem;
		padding: 0.25rem 0.5rem;
		border: 1px solid #3a3a3a;
		border-radius: 0.25rem;
		font-size: 0.875rem;
		color: #c8c8c8;
	}

	.dot {
		width: 0.6rem;
		height: 0.6rem;
		border-radius: 50%;
		background-color: #9a9a9a;
	}

	.online .dot {
		background-color: #2ecc40;
	}

	.offline .dot {
		background-color: #ff4136;
	}

	.footer-link {
		color: #c9d1d9;
		font-size: 0.875rem;
		text-decoration: none;
	}

	.footer-link:hover {
		color: var(--color-blue-light);
	}

	.version {
		color: #9a9a9a;
		font-size: 0.875rem;
	}
</style>
