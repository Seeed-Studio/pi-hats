/* SPDX-License-Identifier: GPL-2.0
 *
 * mcp25xxfd - Microchip MCP25xxFD Family CAN controller driver
 *
 * Copyright (c) 2019 Pengutronix,
 *                    Marc Kleine-Budde <kernel@pengutronix.de>
 */

#ifndef _MCP25XXFD_LOG_H
#define _MCP25XXFD_LOG_H

struct mcp25xxfd_priv;

/*init debug undef*/
//#undef  CONFIG_CAN_MCP25XXFD_LOG

#ifdef CONFIG_CAN_MCP25XXFD_LOG

struct mcp25xxfd_log {
	const char *func;
	canid_t can_id;
	u32 tef_head;
	u32 tef_tail;
	u32 tx_head;
	u32 tx_tail;
	u32 rx_head;
	u32 rx_tail;

#define MCP25XXFD_LOG_STOP BIT(0)
#define MCP25XXFD_LOG_WAKE BIT(1)
#define MCP25XXFD_LOG_BUSY BIT(2)
#define MCP25XXFD_LOG_TXMAB BIT(3)
#define MCP25XXFD_LOG_RXMAB BIT(4)
	u32 flags;

	u32 hw_tef_tail;
	u32 hw_tx_ci;
	u32 hw_rx_head;
	u32 hw_rx_tail;
	u8 rx_offset;
	u8 rx_len;
};

struct mcp25xxfd_log *___mcp25xxfd_log(struct mcp25xxfd_priv *priv, const char *func, canid_t can_id);
void mcp25xxfd_log_dump(const struct mcp25xxfd_priv *priv);

static inline struct mcp25xxfd_log *__mcp25xxfd_log(struct mcp25xxfd_priv *priv, const char *func, canid_t can_id)
{
	struct mcp25xxfd_log *log;

	log = ___mcp25xxfd_log(priv, func, can_id);
	log->hw_tef_tail = -1;
	log->hw_tx_ci = -1;
	log->hw_rx_head = -1;
	log->hw_rx_tail = -1;
	log->rx_offset = -1;
	log->rx_len = -1;

	return log;
}

#define mcp25xxfd_log(priv, ...) __mcp25xxfd_log(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_no_canid(struct mcp25xxfd_priv *priv, const char *func)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log(priv, func, -1);
	log->can_id = -1;

	return log;
}

#define mcp25xxfd_log_no_canid(priv, ...) __mcp25xxfd_log_no_canid(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_stop(struct mcp25xxfd_priv *priv, const char *func, canid_t can_id)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log(priv, func, can_id);
	log->flags |= MCP25XXFD_LOG_STOP;

	return log;
}

#define mcp25xxfd_log_stop(priv, ...) __mcp25xxfd_log_stop(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_wake(struct mcp25xxfd_priv *priv, const char *func, canid_t can_id)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log(priv, func, can_id);
	log->flags |= MCP25XXFD_LOG_WAKE;

	return log;
}

#define mcp25xxfd_log_wake(priv, ...) __mcp25xxfd_log_wake(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_busy(struct mcp25xxfd_priv *priv, const char *func, canid_t can_id)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log(priv, func, can_id);
	log->flags |= MCP25XXFD_LOG_BUSY;

	return log;
}

#define mcp25xxfd_log_busy(priv, ...) __mcp25xxfd_log_busy(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_txmab(struct mcp25xxfd_priv *priv, const char *func)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->flags |= MCP25XXFD_LOG_TXMAB;

	return log;
}

#define mcp25xxfd_log_txmab(priv, ...) __mcp25xxfd_log_txmab(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_rxmab(struct mcp25xxfd_priv *priv, const char *func)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->flags |= MCP25XXFD_LOG_RXMAB;

	return log;
}

#define mcp25xxfd_log_rxmab(priv, ...) __mcp25xxfd_log_rxmab(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_hw_tx_ci(struct mcp25xxfd_priv *priv, const char *func, u32 tx_ci)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->hw_tx_ci = tx_ci;

	return log;
}

#define mcp25xxfd_log_hw_tx_ci(priv, ...) __mcp25xxfd_log_hw_tx_ci(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_hw_tef_tail(struct mcp25xxfd_priv *priv, const char *func, u32 tef_tail)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->hw_tef_tail = tef_tail;

	return log;
}

#define mcp25xxfd_log_hw_tef_tail(priv, ...) __mcp25xxfd_log_hw_tef_tail(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_hw_rx_head(struct mcp25xxfd_priv *priv, const char *func, u32 rx_head)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->hw_rx_head = rx_head;

	return log;
}

#define mcp25xxfd_log_hw_rx_head(priv, ...) __mcp25xxfd_log_hw_rx_head(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_hw_rx_tail(struct mcp25xxfd_priv *priv, const char *func, u32 rx_tail)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->hw_rx_tail = rx_tail;

	return log;
}

#define mcp25xxfd_log_hw_rx_tail(priv, ...) __mcp25xxfd_log_hw_rx_tail(priv, __func__, ## __VA_ARGS__)

static inline struct mcp25xxfd_log *__mcp25xxfd_log_rx_obj_read(struct mcp25xxfd_priv *priv, const char *func, u8 offset, u8 len)
{
	struct mcp25xxfd_log *log;

	log = __mcp25xxfd_log_no_canid(priv, func);
	log->rx_offset = offset;
	log->rx_len = len;

	return log;
}

#define mcp25xxfd_log_rx_obj_read(priv, ...) __mcp25xxfd_log_rx_obj_read(priv, __func__, ## __VA_ARGS__)

#else

static inline void mcp25xxfd_log_dump(const struct mcp25xxfd_priv *priv)
{
}

static inline struct mcp25xxfd_log *mcp25xxfd_log(struct mcp25xxfd_priv *priv, canid_t can_id)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_no_canid(struct mcp25xxfd_priv *priv)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_stop(struct mcp25xxfd_priv *priv, canid_t can_id)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_wake(struct mcp25xxfd_priv *priv, canid_t can_id)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_busy(struct mcp25xxfd_priv *priv, canid_t can_id)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_txmab(struct mcp25xxfd_priv *priv)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_rxmab(struct mcp25xxfd_priv *priv)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_hw_tx_ci(struct mcp25xxfd_priv *priv, u32 tx_ci)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_hw_rx_head(struct mcp25xxfd_priv *priv, u32 rx_head)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_hw_tef_tail(struct mcp25xxfd_priv *priv, u32 tef_tail)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_hw_rx_tail(struct mcp25xxfd_priv *priv, u32 rx_tail)
{
	return NULL;
}

static inline struct mcp25xxfd_log *mcp25xxfd_log_rx_obj_read(struct mcp25xxfd_priv *priv, u8 offset, u8 len)
{
	return NULL;
}

#endif

#endif
