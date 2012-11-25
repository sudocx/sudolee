/*
 * Clock power management.
 */

#include "clk_pwr.h"

inline void delay(u32 loops)
{
	__asm__ __volatile__("0:\n\t"
			     "subs %0, %1, #1\n\t"
			     "nop\n\t" "bne 0b":"=r"(loops):"0"(loops));
}

static inline struct clk_pwr_t *get_cpm_base(void)
{
	return (struct clk_pwr_t *)CLOCK_POWER_ENTRY;
}

void clk_pwr_init(void)
{
	struct clk_pwr_t *clk = get_cpm_base();

	writel(&clk->locktime, 0xffffffff);	/* max pll lock time */

	/* upll clock = 48 MHz, MDIV-PDIV-SDIV */
	writel(&clk->upllcon, (0x38 << 12) | (0x2 << 4) | (0x2 << 0));

	delay(7);

	/* mpll clock = 405 MHz, MDIV-PDIV-SDIV */
	writel(&clk->mpllcon, (0x7f << 12) | (0x2 << 4) | (0x1 << 0));

	delay(7);

	/*
	 * Uclk = UPLL; Fclk:Hclk:Pclk = 1:3:6;
	 * HDIVN-PDIVN
	 */
	writel(&clk->clkdivn, (0x0 << 3) | (0x3 << 1) | (0x1 << 0));

	delay(7);
}
