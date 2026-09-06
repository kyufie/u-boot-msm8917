
#include <clk-uclass.h>
#include <dm/device.h>
#include <linux/kernel.h>
#include <dt-bindings/clock/qcom,gcc-msm8917.h>

#include "clock-qcom.h"

static const struct gate_clk msm8917_clks[] = {
	GATE_CLK_POLLED(GCC_BLSP1_UART1_APPS_CLK,	0x0203c, BIT(0), 0x0203c),
	GATE_CLK_POLLED(GCC_BLSP1_UART2_APPS_CLK,	0x0302c, BIT(0), 0x0302c),
	GATE_CLK_POLLED(GCC_BLSP2_UART1_APPS_CLK,	0x0c03c, BIT(0), 0x0c03c),
	GATE_CLK_POLLED(GCC_BLSP2_UART2_APPS_CLK,	0x0d02c, BIT(0), 0x0d02c),
	GATE_CLK_POLLED(GCC_SDCC1_AHB_CLK,		0x4201c, BIT(0), 0x4201c),
	GATE_CLK_POLLED(GCC_SDCC1_APPS_CLK,		0x42018, BIT(0), 0x42018),
	GATE_CLK_POLLED(GCC_SDCC2_AHB_CLK,		0x4301c, BIT(0), 0x4301c),
	GATE_CLK_POLLED(GCC_SDCC2_APPS_CLK,		0x43018, BIT(0), 0x43018),
	GATE_CLK_POLLED(GCC_USB_HS_PHY_CFG_AHB_CLK,	0x41030, BIT(0), 0x41030),
	GATE_CLK_POLLED(GCC_USB_HS_AHB_CLK,		0x41008, BIT(0), 0x41008),
	GATE_CLK_POLLED(GCC_USB_HS_SYSTEM_CLK,		0x41004, BIT(0), 0x41004),
	GATE_CLK_POLLED(GCC_USB2A_PHY_SLEEP_CLK,	0x4102c, BIT(0), 0x4102c),
};

static const struct freq_tbl ftbl_blsp_uart_apps_clk_src[] = {
	F(3686400, CFG_CLK_SRC_GPLL0, 1, 72, 15625),
	F(7372800, CFG_CLK_SRC_GPLL0, 1, 144, 15625),
	F(14745600, CFG_CLK_SRC_GPLL0, 1, 288, 15625),
	F(16000000, CFG_CLK_SRC_GPLL0, 10, 1, 5),
	F(19200000, CFG_CLK_SRC_CXO, 1, 0, 0),
	F(24000000, CFG_CLK_SRC_GPLL0, 1, 3, 100),
	F(25000000, CFG_CLK_SRC_GPLL0, 16, 1, 2),
	F(32000000, CFG_CLK_SRC_GPLL0, 1, 1, 25),
	F(40000000, CFG_CLK_SRC_GPLL0, 1, 1, 20),
	F(46400000, CFG_CLK_SRC_GPLL0, 1, 29, 500),
	F(48000000, CFG_CLK_SRC_GPLL0, 1, 3, 50),
	F(51200000, CFG_CLK_SRC_GPLL0, 1, 8, 125),
	F(56000000, CFG_CLK_SRC_GPLL0, 1, 7, 100),
	F(58982400, CFG_CLK_SRC_GPLL0, 1, 1152, 15625),
	F(60000000, CFG_CLK_SRC_GPLL0, 1, 3, 40),
	F(64000000, CFG_CLK_SRC_GPLL0, 1, 2, 25),
	{ }
};

static const struct freq_tbl ftbl_sdcc1_apps_clk_src[] = {
	F(144000, CFG_CLK_SRC_CXO, 16, 3, 25),
	F(400000, CFG_CLK_SRC_CXO, 12, 1, 4),
	F(20000000, CFG_CLK_SRC_GPLL0, 10, 1, 4),
	F(25000000, CFG_CLK_SRC_GPLL0, 16, 1, 2),
	F(50000000, CFG_CLK_SRC_GPLL0, 16, 0, 0),
	F(100000000, CFG_CLK_SRC_GPLL0, 8, 0, 0),
	F(177770000, CFG_CLK_SRC_GPLL0, 4.5, 0, 0),
	F(192000000, CFG_CLK_SRC_GPLL4_EVEN, 6, 0, 0),
	F(200000000, CFG_CLK_SRC_GPLL0, 4, 0, 0),
	F(384000000, CFG_CLK_SRC_GPLL4_EVEN, 3, 0, 0),
	{ }
};

static const struct freq_tbl ftbl_sdcc2_apps_clk_src[] = {
	F(144000, CFG_CLK_SRC_CXO, 16, 3, 25),
	F(400000, CFG_CLK_SRC_CXO, 12, 1, 4),
	F(20000000, CFG_CLK_SRC_GPLL0, 10, 1, 4),
	F(25000000, CFG_CLK_SRC_GPLL0, 16, 1, 2),
	F(50000000, CFG_CLK_SRC_GPLL0, 16, 0, 0),
	F(100000000, CFG_CLK_SRC_GPLL0, 8, 0, 0),
	F(177770000, CFG_CLK_SRC_GPLL0, 4.5, 0, 0),
	F(200000000, CFG_CLK_SRC_GPLL0, 4, 0, 0),
	{ }
};

static const struct freq_tbl ftbl_usb_hs_system_clk_src[] = {
	F(80000000, CFG_CLK_SRC_GPLL0, 10, 0, 0),
	F(100000000, CFG_CLK_SRC_GPLL0, 8, 0, 0),
	F(133330000, CFG_CLK_SRC_GPLL0, 6, 0, 0),
	F(177780000, CFG_CLK_SRC_GPLL0, 4.5, 0, 0),
	{ }
};

static int msm8917_enable(struct clk *clk)
{
	struct msm_clk_priv *priv;

	priv = dev_get_priv(clk->dev);
	return qcom_gate_clk_en(priv, clk->id);
}

static ulong msm8917_uart_set_rate(struct clk *clk, ulong rate)
{
	struct msm_clk_priv *priv = dev_get_priv(clk->dev);
	const struct freq_tbl *freq;
	u32 cmd_rcgr;

	switch (clk->id) {
	case GCC_BLSP1_UART1_APPS_CLK:
		cmd_rcgr = 0x02044;
		break;
	case GCC_BLSP1_UART2_APPS_CLK:
		cmd_rcgr = 0x03034;
		break;
	case GCC_BLSP2_UART1_APPS_CLK:
		cmd_rcgr = 0x0c044;
		break;
	case GCC_BLSP2_UART2_APPS_CLK:
		cmd_rcgr = 0x0d034;
		break;
	default:
		return 0;
	}

	freq = qcom_find_freq(ftbl_blsp_uart_apps_clk_src, rate);
	clk_rcg_set_rate_mnd(priv->base, cmd_rcgr,
				 freq->pre_div, freq->m, freq->n, freq->src, 16);

	return freq->freq;
}

static ulong msm8917_set_rate(struct clk *clk, ulong rate)
{
	struct msm_clk_priv *priv = dev_get_priv(clk->dev);
	const struct freq_tbl *freq;

	switch (clk->id) {
	case GCC_BLSP1_UART1_APPS_CLK:
	case GCC_BLSP1_UART2_APPS_CLK:
	case GCC_BLSP2_UART1_APPS_CLK:
	case GCC_BLSP2_UART2_APPS_CLK:
		return msm8917_uart_set_rate(clk, rate);
	case GCC_SDCC1_APPS_CLK:
		freq = qcom_find_freq(ftbl_sdcc1_apps_clk_src, rate);
		clk_rcg_set_rate_mnd(priv->base, 0x42004,
				     freq->pre_div, freq->m, freq->n, freq->src, 8);
		return freq->freq;
	case GCC_SDCC2_APPS_CLK:
		freq = qcom_find_freq(ftbl_sdcc2_apps_clk_src, rate);
		clk_rcg_set_rate_mnd(priv->base, 0x43004,
				     freq->pre_div, freq->m, freq->n, freq->src, 8);
		return freq->freq;
	case GCC_USB_HS_SYSTEM_CLK:
		freq = qcom_find_freq(ftbl_usb_hs_system_clk_src, rate);
		clk_rcg_set_rate_mnd(priv->base, 0x41010,
				     freq->pre_div, freq->m, freq->n, freq->src, 5);
		return freq->freq;
	default:
		break;
	}

	return 0;
}

static const struct qcom_reset_map msm8917_resets[] = {
	[GCC_CAMSS_MICRO_BCR]		= { 0x56008 },
	[GCC_MSS_BCR]			= { 0x71000 },
	[GCC_QUSB2_PHY_BCR]		= { 0x4103c },
	[GCC_USB_HS_BCR]		= { 0x41000 },
	[GCC_USB2_HS_PHY_ONLY_BCR]	= { 0x41034 },
	[GCC_MDSS_BCR]			= { 0x4d074 },
};

static const struct qcom_power_map msm8917_gdscs[] = {
	[MDSS_GDSC] = { 0x4d078, true },
};

static struct msm_clk_data msm8917_gcc_data = {
	.resets = msm8917_resets,
	.num_resets = ARRAY_SIZE(msm8917_resets),
	.clks = msm8917_clks,
	.num_clks = ARRAY_SIZE(msm8917_clks),
	.power_domains = msm8917_gdscs,
	.num_power_domains = ARRAY_SIZE(msm8917_gdscs),

	.enable = msm8917_enable,
	.set_rate = msm8917_set_rate,
};

static const struct udevice_id gcc_msm8917_of_match[] = {
	{
		.compatible = "qcom,gcc-msm8917",
		.data = (ulong)&msm8917_gcc_data,
	},
	{ }
};

U_BOOT_DRIVER(gcc_msm8917) = {
	.name		= "gcc_msm8917",
	.id		= UCLASS_NOP,
	.of_match	= gcc_msm8917_of_match,
	.bind		= qcom_cc_bind,
	.flags		= DM_FLAG_PRE_RELOC | DM_FLAG_DEFAULT_PD_CTRL_OFF,
};
