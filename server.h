struct parlor{
	uint16_t pizza1_dollars;
	uint16_t pizza1_cents;
	uint16_t pizza1_inches;
	uint16_t pizza1_fractional_inches;
	uint16_t pizza2_dollars;
	uint16_t pizza2_cents;
	uint16_t pizza2_inches;
	uint16_t pizza2_fractional_inches;
	uint16_t vendor1_name_ln;
	uint16_t vendor2_name_ln;
} __attribute__ ((packed));


struct calc{
	uint16_t p1dol;
	uint16_t p1cen;
	uint16_t p2dol;
	uint16_t p2cen;
	uint16_t win_len;
} __attribute__ ((packed));
