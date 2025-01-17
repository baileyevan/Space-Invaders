
@{{BLOCK(alien_bn_gfx)

@=======================================================================
@
@	alien_bn_gfx, 16x16@4, 
@	+ palette 16 entries, not compressed
@	+ 4 tiles not compressed
@	Total size: 32 + 128 = 160
@
@	Time-stamp: 2024-11-25, 00:19:14
@	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global alien_bn_gfxTiles		@ 128 unsigned chars
	.hidden alien_bn_gfxTiles
alien_bn_gfxTiles:
	.word 0x00000000,0x00000000,0x00000000,0x20000000,0x12000000,0x11200000,0x41200000,0x11200000
	.word 0x00000000,0x00000000,0x00000000,0x00000002,0x00000021,0x00000211,0x00000214,0x00000211
	.word 0x12200000,0x12200000,0x11203000,0x11133300,0x33333000,0x00000000,0x00000000,0x00000000
	.word 0x00000221,0x00000221,0x00030211,0x00333211,0x00033333,0x00000000,0x00000000,0x00000000

	.section .rodata
	.align	2
	.global alien_bn_gfxPal		@ 32 unsigned chars
	.hidden alien_bn_gfxPal
alien_bn_gfxPal:
	.hword 0x0000,0x03EB,0x7FFF,0x2529,0x001F,0x0000,0x0000,0x0000
	.hword 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000

@}}BLOCK(alien_bn_gfx)
