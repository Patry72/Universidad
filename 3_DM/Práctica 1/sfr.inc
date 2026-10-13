;****************************************************************
;*																*
;*		M16C/60 SFR definition for M16C starter kit				*
;*																*
;*									1996.11.20					*
;*																*
;****************************************************************
;																 
;  Copyright,1996												 
;	MITSUBISHI ERECTRIC CORPORATION AND							 
;       MITSUBISHI ERECTRIC SEMICONDUCTOR SOFTWARE CORPORATIOON	 
;																 
;----------------------------------------------------------------
;	Processor mode register 0									 
;----------------------------------------------------------------
PM0			.EQU		0004H
;
PM00		.BTEQU		0,PM0
PM01		.BTEQU		1,PM0
PM02		.BTEQU		2,PM0
PM03		.BTEQU		3,PM0
PM04		.BTEQU		4,PM0
PM05		.BTEQU		5,PM0
PM06		.BTEQU		6,PM0
PM07		.BTEQU		7,PM0
;
;----------------------------------------------------------------
;	Processor mode register 1									 
;----------------------------------------------------------------
PM1			.EQU		0005H
;
PM10		.BTEQU		0,PM1
;
;----------------------------------------------------------------
;	System clock control register 0								 
;----------------------------------------------------------------
CM0			.EQU		0006H
;
CM00		.BTEQU		0,CM0
CM01		.BTEQU		1,CM0
CM02		.BTEQU		2,CM0
CM03		.BTEQU		3,CM0
CM04		.BTEQU		4,CM0
CM05		.BTEQU		5,CM0
CM06		.BTEQU		6,CM0
CM07		.BTEQU		7,CM0
;
;----------------------------------------------------------------
;	System clock control register 1								 
;----------------------------------------------------------------
CM1			.EQU		0007H
;
CM10		.BTEQU		0,CM1
CM15		.BTEQU		5,CM1
CM16		.BTEQU		6,CM1
CM17		.BTEQU		7,CM1
;
;----------------------------------------------------------------
;	Chip select control register								 
;----------------------------------------------------------------
CSR			.EQU		0008H
;
CS0			.BTEQU		0,CSR
CS1			.BTEQU		1,CSR
CS2			.BTEQU		2,CSR
CS3			.BTEQU		3,CSR
CS0W		.BTEQU		4,CSR
CS1W		.BTEQU		5,CSR
CS2W		.BTEQU		6,CSR
CS3W		.BTEQU		7,CSR
;
;----------------------------------------------------------------
;	Address match interrupt enable register						 
;----------------------------------------------------------------
AIER		.EQU		0009H
;
AIER0		.BTEQU		0,AIER
AIER1		.BTEQU		1,AIER
;
;----------------------------------------------------------------
;	Protect register											 
;----------------------------------------------------------------
PRCR		.EQU		000AH
;
PRC0		.BTEQU		0,PRCR
PRC1		.BTEQU		1,PRCR
PRC2		.BTEQU		2,PRCR
;
;----------------------------------------------------------------
;	Watchdog timer register										 
;----------------------------------------------------------------
WDTS		.EQU		000EH
;
WDC			.EQU		000FH
;
WDC5		.BTEQU		5,WDC
WDC6		.BTEQU		6,WDC
WDC7		.BTEQU		7,WDC
;
;----------------------------------------------------------------
;	Address match interrupt register 0							 
;----------------------------------------------------------------
RMAD0		.EQU		0010H
RMAD0L		.EQU		RMAD0
RMAD0M		.EQU		RMAD0+1
RMAD0H		.EQU		RMAD0+2
;
;----------------------------------------------------------------
;	Address match interrupt register 1							 
;----------------------------------------------------------------
RMAD1		.EQU		0014H
RMAD1L		.EQU		RMAD1
RMAD1M		.EQU		RMAD1+1
RMAD1H		.EQU		RMAD1+2
;
;----------------------------------------------------------------
;	DMA0 source pointer											 
;----------------------------------------------------------------
SAR0		.EQU		0020H
SAR0L		.EQU		SAR0
SAR0M		.EQU		SAR0+1
SAR0H		.EQU		SAR0+2
;
;----------------------------------------------------------------
;	DMA0 destination pointer									 
;----------------------------------------------------------------
DAR0		.EQU		0024H
DAR0L		.EQU		DAR0
DAR0M		.EQU		DAR0+1
DAR0H		.EQU		DAR0+2
;
;----------------------------------------------------------------
;	DMA0 transfer counter										 
;----------------------------------------------------------------
TCR0		.EQU		0028H
TCR0L		.EQU		TCR0
TCR0H		.EQU		TCR0+1
;
;----------------------------------------------------------------
;	DMA0 control register										 
;----------------------------------------------------------------
DM0CON		.EQU		002CH
;
DMBIT_DM0CON	.BTEQU		0,DM0CON
DMASL_DM0CON	.BTEQU		1,DM0CON
DMAS_DM0CON	.BTEQU		2,DM0CON
DMAE_DM0CON	.BTEQU		3,DM0CON
DSD_DM0CON	.BTEQU		4,DM0CON
DAD_DM0CON	.BTEQU		5,DM0CON
;
;----------------------------------------------------------------
;	DMA1 source pointer											 
;----------------------------------------------------------------
SAR1		.EQU		0030H
SAR1L		.EQU		SAR1
SAR1M		.EQU		SAR1+1
SAR1H		.EQU		SAR1+2
;
;----------------------------------------------------------------
;	DMA1 destination pointer									 
;----------------------------------------------------------------
DAR1		.EQU		0034H
DAR1L		.EQU		DAR1
DAR1M		.EQU		DAR1+1
DAR1H		.EQU		DAR1+2
;
;----------------------------------------------------------------
;	DMA1 transfer counter										 
;----------------------------------------------------------------
TCR1		.EQU		0038H
TCR1L		.EQU		TCR1
TCR1H		.EQU		TCR1+1
;
;----------------------------------------------------------------
;	DMA1 control register										 
;----------------------------------------------------------------
DM1CON		.EQU		003CH
;
DMBIT_DM1CON	.BTEQU		0,DM1CON
DMASL_DM1CON	.BTEQU		1,DM1CON
DMAS_DM1CON	.BTEQU		2,DM1CON
DMAE_DM1CON	.BTEQU		3,DM1CON
DSD_DM1CON	.BTEQU		4,DM1CON
DAD_DM1CON	.BTEQU		5,DM1CON
;
;----------------------------------------------------------------
;	Interrupt control register									 
;----------------------------------------------------------------
DM0IC		.EQU		004BH
ILVL0_DM0IC	.BTEQU		0,DM0IC
ILVL1_DM0IC	.BTEQU		1,DM0IC
ILVL2_DM0IC	.BTEQU		2,DM0IC
IR_DM0IC	.BTEQU		3,DM0IC
;
DM1IC		.EQU		004CH
ILVL0_DM1IC	.BTEQU		0,DM1IC
ILVL1_DM1IC	.BTEQU		1,DM1IC
ILVL2_DM1IC	.BTEQU		2,DM1IC
IR_DM1IC	.BTEQU		3,DM1IC
;
KUPIC		.EQU		004DH
ILVL0_KUPIC	.BTEQU		0,KUPIC
ILVL1_KUPIC	.BTEQU		1,KUPIC
ILVL2_KUPIC	.BTEQU		2,KUPIC
IR_KUPIC	.BTEQU		3,KUPIC
;
ADIC		.EQU		004EH
ILVL0_ADIC	.BTEQU		0,ADIC
ILVL1_ADIC	.BTEQU		1,ADIC
ILVL2_ADIC	.BTEQU		2,ADIC
IR_ADIC		.BTEQU		3,ADIC
;
S0TIC		.EQU		0051H
ILVL0_S0TIC	.BTEQU		0,S0TIC
ILVL1_S0TIC	.BTEQU		1,S0TIC
ILVL2_S0TIC	.BTEQU		2,S0TIC
IR_S0TIC	.BTEQU		3,S0TIC
;
S0RIC		.EQU		0052H
ILVL0_S0RIC	.BTEQU		0,S0RIC
ILVL1_S0RIC	.BTEQU		1,S0RIC
ILVL2_S0RIC	.BTEQU		2,S0RIC
IR_S0RIC	.BTEQU		3,S0RIC
;
S1TIC		.EQU		0053H
ILVL0_S1TIC	.BTEQU		0,S1TIC
ILVL1_S1TIC	.BTEQU		1,S1TIC
ILVL2_S1TIC	.BTEQU		2,S1TIC
IR_S1TIC	.BTEQU		3,S1TIC
;
S1RIC		.EQU		0054H
ILVL0_S1RIC	.BTEQU		0,S1RIC
ILVL1_S1RIC	.BTEQU		1,S1RIC
ILVL2_S1RIC	.BTEQU		2,S1RIC
IR_S1RIC	.BTEQU		3,S1RIC
;
TA0IC		.EQU		0055H
ILVL0_TA0IC	.BTEQU		0,TA0IC
ILVL1_TA0IC	.BTEQU		1,TA0IC
ILVL2_TA0IC	.BTEQU		2,TA0IC
IR_TA0IC	.BTEQU		3,TA0IC
;
TA1IC		.EQU		0056H
ILVL0_TA1IC	.BTEQU		0,TA1IC
ILVL1_TA1IC	.BTEQU		1,TA1IC
ILVL2_TA1IC	.BTEQU		2,TA1IC
IR_TA1IC	.BTEQU		3,TA1IC
;
TA2IC		.EQU		0057H
ILVL0_TA2IC	.BTEQU		0,TA2IC
ILVL1_TA2IC	.BTEQU		1,TA2IC
ILVL2_TA2IC	.BTEQU		2,TA2IC
IR_TA2IC	.BTEQU		3,TA2IC
;
TA3IC		.EQU		0058H
ILVL0_TA3IC	.BTEQU		0,TA3IC
ILVL1_TA3IC	.BTEQU		1,TA3IC
ILVL2_TA3IC	.BTEQU		2,TA3IC
IR_TA3IC	.BTEQU		3,TA3IC
;
TA4IC		.EQU		0059H
ILVL0_TA4IC	.BTEQU		0,TA4IC
ILVL1_TA4IC	.BTEQU		1,TA4IC
ILVL2_TA4IC	.BTEQU		2,TA4IC
IR_TA4IC	.BTEQU		3,TA4IC
;
TB0IC		.EQU		005AH
ILVL0_TB0IC	.BTEQU		0,TB0IC
ILVL1_TB0IC	.BTEQU		1,TB0IC
ILVL2_TB0IC	.BTEQU		2,TB0IC
IR_TB0IC	.BTEQU		3,TB0IC
;
TB1IC		.EQU		005BH
ILVL0_TB1IC	.BTEQU		0,TB1IC
ILVL1_TB1IC	.BTEQU		1,TB1IC
ILVL2_TB1IC	.BTEQU		2,TB1IC
IR_TB1IC	.BTEQU		3,TB1IC
;
TB2IC		.EQU		005CH
ILVL0_TB2IC	.BTEQU		0,TB2IC
ILVL1_TB2IC	.BTEQU		1,TB2IC
ILVL2_TB2IC	.BTEQU		2,TB2IC
IR_TB2IC	.BTEQU		3,TB2IC
;
INT0IC		.EQU		005DH
ILVL0_INT0IC	.BTEQU		0,INT0IC
ILVL1_INT0IC	.BTEQU		1,INT0IC
ILVL2_INT0IC	.BTEQU		2,INT0IC
IR_INT0IC	.BTEQU		3,INT0IC
POL_INT0IC	.BTEQU		4,INT0IC
LVS_INT0IC	.BTEQU		5,INT0IC
;
INT1IC		.EQU		005EH
ILVL0_INT1IC	.BTEQU		0,INT1IC
ILVL1_INT1IC	.BTEQU		1,INT1IC
ILVL2_INT1IC	.BTEQU		2,INT1IC
IR_INT1IC	.BTEQU		3,INT1IC
POL_INT1IC	.BTEQU		4,INT1IC
LVS_INT1IC	.BTEQU		5,INT1IC
;
INT2IC		.EQU		005FH
ILVL0_INT2IC	.BTEQU		0,INT2IC
ILVL1_INT2IC	.BTEQU		1,INT2IC
ILVL2_INT2IC	.BTEQU		2,INT2IC
IR_INT2IC	.BTEQU		3,INT2IC
POL_INT2IC	.BTEQU		4,INT2IC
LVS_INT2IC	.BTEQU		5,INT2IC
;
;----------------------------------------------------------------
;	Count start flag											 
;----------------------------------------------------------------
TABSR		.EQU		0380H
;
TA0S		.BTEQU		0,TABSR
TA1S		.BTEQU		1,TABSR
TA2S		.BTEQU		2,TABSR
TA3S		.BTEQU		3,TABSR
TA4S		.BTEQU		4,TABSR
TB0S		.BTEQU		5,TABSR
TB1S		.BTEQU		6,TABSR
TB2S		.BTEQU		7,TABSR
;
;----------------------------------------------------------------
;	Clock prescaler reset flag									 
;----------------------------------------------------------------
CPSRF		.EQU		0381H
;
CPSR		.BTEQU		7,CPSRF
;
;----------------------------------------------------------------
;	One shot start flag											 
;----------------------------------------------------------------
ONSF		.EQU		0382H
;
TA0OS		.BTEQU		0,ONSF
TA1OS		.BTEQU		1,ONSF
TA2OS		.BTEQU		2,ONSF
TA3OS		.BTEQU		3,ONSF
TA4OS		.BTEQU		4,ONSF
TA0TGL		.BTEQU		6,ONSF
TA0TGH		.BTEQU		7,ONSF
;
;----------------------------------------------------------------
;	Trigger select register										 
;----------------------------------------------------------------
TRGSR		.EQU		0383H
;
TA1TGL		.BTEQU		0,TRGSR
TA1TGH		.BTEQU		1,TRGSR
TA2TGL		.BTEQU		2,TRGSR
TA2TGH		.BTEQU		3,TRGSR
TA3TGL		.BTEQU		4,TRGSR
TA3TGH		.BTEQU		5,TRGSR
TA4TGL		.BTEQU		6,TRGSR
TA4TGH		.BTEQU		7,TRGSR
;
;----------------------------------------------------------------
;	Up down flag												 
;----------------------------------------------------------------
UDF			.EQU		0384H
;
TA0UD		.BTEQU		0,UDF
TA1UD		.BTEQU		1,UDF
TA2UD		.BTEQU		2,UDF
TA3UD		.BTEQU		3,UDF
TA4UD		.BTEQU		4,UDF
TA2P		.BTEQU		5,UDF
TA3P		.BTEQU		6,UDF
TA4P		.BTEQU		7,UDF
;
;----------------------------------------------------------------
;	Timer														 
;----------------------------------------------------------------
TA0			.EQU		0386H
TA0L		.EQU		TA0
TA0H		.EQU		TA0+1
;
TA1			.EQU		0388H
TA1L		.EQU		TA1
TA1H		.EQU		TA1+1
;
TA2			.EQU		038AH
TA2L		.EQU		TA2
TA2H		.EQU		TA2+1
;
TA3			.EQU		038CH
TA3L		.EQU		TA3
TA3H		.EQU		TA3+1
;
TA4			.EQU		038EH
TA4L		.EQU		TA4
TA4H		.EQU		TA4+1
;
TB0			.EQU		0390H
TB0L		.EQU		TB0
TB0H		.EQU		TB0+1
;
TB1			.EQU		0392H
TB1L		.EQU		TB1
TB1H		.EQU		TB1+1
;
TB2			.EQU		0394H
TB2L		.EQU		TB2
TB2H		.EQU		TB2+1
;
TA0MR		.EQU		0396H
TMOD0_TA0MR	.BTEQU		0,TA0MR
TMOD1_TA0MR	.BTEQU		1,TA0MR
MR0_TA0MR	.BTEQU		2,TA0MR
MR1_TA0MR	.BTEQU		3,TA0MR
MR2_TA0MR	.BTEQU		4,TA0MR
MR3_TA0MR	.BTEQU		5,TA0MR
TCK0_TA0MR	.BTEQU		6,TA0MR
TCK1_TA0MR	.BTEQU		7,TA0MR
;
TA1MR		.EQU		0397H
TMOD0_TA1MR	.BTEQU		0,TA1MR
TMOD1_TA1MR	.BTEQU		1,TA1MR
MR0_TA1MR	.BTEQU		2,TA1MR
MR1_TA1MR	.BTEQU		3,TA1MR
MR2_TA1MR	.BTEQU		4,TA1MR
MR3_TA1MR	.BTEQU		5,TA1MR
TCK0_TA1MR	.BTEQU		6,TA1MR
TCK1_TA1MR	.BTEQU		7,TA1MR
;
TA2MR		.EQU		0398H
TMOD0_TA2MR	.BTEQU		0,TA2MR
TMOD1_TA2MR	.BTEQU		1,TA2MR
MR0_TA2MR	.BTEQU		2,TA2MR
MR1_TA2MR	.BTEQU		3,TA2MR
MR2_TA2MR	.BTEQU		4,TA2MR
MR3_TA2MR	.BTEQU		5,TA2MR
TCK0_TA2MR	.BTEQU		6,TA2MR
TCK1_TA2MR	.BTEQU		7,TA2MR
;
TA3MR		.EQU		0399H
TMOD0_TA3MR	.BTEQU		0,TA3MR
TMOD1_TA3MR	.BTEQU		1,TA3MR
MR0_TA3MR	.BTEQU		2,TA3MR
MR1_TA3MR	.BTEQU		3,TA3MR
MR2_TA3MR	.BTEQU		4,TA3MR
MR3_TA3MR	.BTEQU		5,TA3MR
TCK0_TA3MR	.BTEQU		6,TA3MR
TCK1_TA3MR	.BTEQU		7,TA3MR
;
TA4MR		.EQU		039AH
TMOD0_TA4MR	.BTEQU		0,TA4MR
TMOD1_TA4MR	.BTEQU		1,TA4MR
MR0_TA4MR	.BTEQU		2,TA4MR
MR1_TA4MR	.BTEQU		3,TA4MR
MR2_TA4MR	.BTEQU		4,TA4MR
MR3_TA4MR	.BTEQU		5,TA4MR
TCK0_TA4MR	.BTEQU		6,TA4MR
TCK1_TA4MR	.BTEQU		7,TA4MR
;
TB0MR		.EQU		039BH
TMOD0_TB0MR	.BTEQU		0,TB0MR
TMOD1_TB0MR	.BTEQU		1,TB0MR
MR0_TB0MR	.BTEQU		2,TB0MR
MR1_TB0MR	.BTEQU		3,TB0MR
MR2_TB0MR	.BTEQU		4,TB0MR
MR3_TB0MR	.BTEQU		5,TB0MR
TCK0_TB0MR	.BTEQU		6,TB0MR
TCK1_TB0MR	.BTEQU		7,TB0MR
;
TB1MR		.EQU		039CH
TMOD0_TB1MR	.BTEQU		0,TB1MR
TMOD1_TB1MR	.BTEQU		1,TB1MR
MR0_TB1MR	.BTEQU		2,TB1MR
MR1_TB1MR	.BTEQU		3,TB1MR
MR2_TB1MR	.BTEQU		4,TB1MR
MR3_TB1MR	.BTEQU		5,TB1MR
TCK0_TB1MR	.BTEQU		6,TB1MR
TCK1_TB1MR	.BTEQU		7,TB1MR
;
TB2MR		.EQU		039DH
TMOD0_TB2MR	.BTEQU		0,TB2MR
TMOD1_TB2MR	.BTEQU		1,TB2MR
MR0_TB2MR	.BTEQU		2,TB2MR
MR1_TB2MR	.BTEQU		3,TB2MR
MR2_TB2MR	.BTEQU		4,TB2MR
MR3_TB2MR	.BTEQU		5,TB2MR
TCK0_TB2MR	.BTEQU		6,TB2MR
TCK1_TB2MR	.BTEQU		7,TB2MR
;
;----------------------------------------------------------------
;	UART0														 
;----------------------------------------------------------------
U0MR		.EQU		03A0H
SMD0_U0MR	.BTEQU		0,U0MR
SMD1_U0MR	.BTEQU		1,U0MR
SMD2_U0MR	.BTEQU		2,U0MR
CKDIR_U0MR	.BTEQU		3,U0MR
STPS_U0MR	.BTEQU		4,U0MR
PRY_U0MR	.BTEQU		5,U0MR
PRVE_U0MR	.BTEQU		6,U0MR
SLEP_U0MR	.BTEQU		7,U0MR
;
U0BRG		.EQU		03A1H
;
U0TB		.EQU		03A2H
U0TBL		.EQU		U0TB
U0TBH		.EQU		U0TB+1
;
U0C0		.EQU		03A4H
CLK0_U0C0	.BTEQU		0,U0C0
CLK1_U0C0	.BTEQU		1,U0C0
CRS_U0C0	.BTEQU		2,U0C0
TXEPT_U0C0	.BTEQU		3,U0C0
CRD_U0C0	.BTEQU		4,U0C0
NCH_U0C0	.BTEQU		5,U0C0
CKPOL_U0C0	.BTEQU		6,U0C0
UFORM_U0C0	.BTEQU		7,U0C0
;
U0C1		.EQU		03A5H
TE_U0C1		.BTEQU		0,U0C1
TI_U0C1		.BTEQU		1,U0C1
RE_U0C1		.BTEQU		2,U0C1
RI_U0C1		.BTEQU		3,U0C1
;
U0RB		.EQU		03A6H
U0RBL		.EQU		U0RB
U0RBH		.EQU		U0RB+1
OER_U0RB	.BTEQU		4,U0RBH
FER_U0RB	.BTEQU		5,U0RBH
PER_U0RB	.BTEQU		6,U0RBH
SUM_U0RB	.BTEQU		7,U0RBH
;
;----------------------------------------------------------------
;	UART1														 
;----------------------------------------------------------------
U1MR		.EQU		03A8H
SMD0_U1MR	.BTEQU		0,U1MR
SMD1_U1MR	.BTEQU		1,U1MR
SMD2_U1MR	.BTEQU		2,U1MR
CKDIR_U1MR	.BTEQU		3,U1MR
STPS_U1MR	.BTEQU		4,U1MR
PRY_U1MR	.BTEQU		5,U1MR
PRVE_U1MR	.BTEQU		6,U1MR
SLEP_U1MR	.BTEQU		7,U1MR
;
U1BRG		.EQU		03A9H
;
U1TB		.EQU		03AAH
U1TBL		.EQU		U1TB
U1TBH		.EQU		U1TB+1
;
U1C0		.EQU		03ACH
CLK0_U1C0	.BTEQU		0,U1C0
CLK1_U1C0	.BTEQU		1,U1C0
CRS_U1C0	.BTEQU		2,U1C0
TXEPT_U1C0	.BTEQU		3,U1C0
CRD_U1C0	.BTEQU		4,U1C0
NCH_U1C0	.BTEQU		5,U1C0
CKPOL_U1C0	.BTEQU		6,U1C0
UFORM_U1C0	.BTEQU		7,U1C0
;
U1C1		.EQU		03ADH
TE_U1C1		.BTEQU		0,U1C1
TI_U1C1		.BTEQU		1,U1C1
RE_U1C1		.BTEQU		2,U1C1
RI_U1C1		.BTEQU		3,U1C1
;
U1RB		.EQU		03AEH
U1RBL		.EQU		U1RB
U1RBH		.EQU		U1RB+1
OER_U1RB	.BTEQU		4,U1RBH
FER_U1RB	.BTEQU		5,U1RBH
PER_U1RB	.BTEQU		6,U1RBH
SUM_U1RB	.BTEQU		7,U1RBH
;
UCON		.EQU		03B0H
U0IRS		.BTEQU		0,UCON
U1IRS		.BTEQU		1,UCON
U0RRM		.BTEQU		2,UCON
U1RRM		.BTEQU		3,UCON
CLKMD0		.BTEQU		4,UCON
CLKMD1		.BTEQU		5,UCON
RCSP		.BTEQU		6,UCON
;
DM0SL		.EQU		03B8H
DSEL0_DM0SL	.BTEQU		0,DM0SL
DSEL1_DM0SL	.BTEQU		1,DM0SL
DSEL2_DM0SL	.BTEQU		2,DM0SL
DSEL3_DM0SL	.BTEQU		3,DM0SL
DSR_DM0SL	.BTEQU		7,DM0SL
;
DM1SL		.EQU		03BAH
DSEL0_DM1SL	.BTEQU		0,DM1SL
DSEL1_DM1SL	.BTEQU		1,DM1SL
DSEL2_DM1SL	.BTEQU		2,DM1SL
DSEL3_DM1SL	.BTEQU		3,DM1SL
DSR_DM1SL	.BTEQU		7,DM1SL
;----------------------------------------------------------------
;	CRC															 
;----------------------------------------------------------------
CRCD		.EQU		03BCH
CRCDL		.EQU		CRCD
CRCDH		.EQU		CRCD+1
;
CRCIN		.EQU		03BEH
;
;----------------------------------------------------------------
;	A/D,D/A														 
;----------------------------------------------------------------
AD0	 		.EQU		03C0H
AD0L		.EQU		AD0
AD0H		.EQU		AD0+1
;
AD1			.EQU		03C2H
AD1L		.EQU		AD1
AD1H		.EQU		AD1+1
;
AD2			.EQU		03C4H
AD2L		.EQU		AD2
AD2H		.EQU		AD2+1
;
AD3			.EQU		03C6H
AD3L		.EQU		AD3
AD3H		.EQU		AD3+1
;
AD4			.EQU		03C8H
AD4L		.EQU		AD4
AD4H		.EQU		AD4+1
;
AD5			.EQU		03CAH
AD5L		.EQU		AD5
AD5H		.EQU		AD5+1
;
AD6			.EQU		03CCH
AD6L		.EQU		AD6
AD6H		.EQU		AD6+1
;
AD7			.EQU		03CEH
AD7L		.EQU		AD7
AD7H		.EQU		AD7+1
;
ADCON2		.EQU		03D4H
SMP			.BTEQU		0,ADCON2
;
ADCON0		.EQU		03D6H
CH0			.BTEQU		0,ADCON0
CH1			.BTEQU		1,ADCON0
CH2			.BTEQU		2,ADCON0
MD0			.BTEQU		3,ADCON0
MD1			.BTEQU		4,ADCON0
TRG			.BTEQU		5,ADCON0
ADST		.BTEQU		6,ADCON0
CKS0		.BTEQU		7,ADCON0
;
ADCON1		.EQU		03D7H
SCAN0		.BTEQU		0,ADCON1
SCAN1		.BTEQU		1,ADCON1
MD2			.BTEQU		2,ADCON1
BITS		.BTEQU		3,ADCON1
CKS1		.BTEQU		4,ADCON1
VCUT		.BTEQU		5,ADCON1
OPA0		.BTEQU		6,ADCON1
OPA1		.BTEQU		7,ADCON1
;
;-----------------------------------------------
;
DA0			.EQU		03D8H
;
DA1			.EQU		03DAH
;
DACON		.EQU		03DCH
DA0E		.BTEQU		0,DACON
DA1E		.BTEQU		1,DACON
;
;
;----------------------------------------------------------------
;	Port														 
;----------------------------------------------------------------
;
P0			.EQU		03E0H
P0_0		.BTEQU		0,P0
P0_1		.BTEQU		1,P0
P0_2		.BTEQU		2,P0
P0_3		.BTEQU		3,P0
P0_4		.BTEQU		4,P0
P0_5		.BTEQU		5,P0
P0_6		.BTEQU		6,P0
P0_7		.BTEQU		7,P0
;
P1			.EQU		03E1H
P1_0		.BTEQU		0,P1
P1_1		.BTEQU		1,P1
P1_2		.BTEQU		2,P1
P1_3		.BTEQU		3,P1
P1_4		.BTEQU		4,P1
P1_5		.BTEQU		5,P1
P1_6		.BTEQU		6,P1
P1_7		.BTEQU		7,P1
;
PD0			.EQU		03E2H
PD0_0		.BTEQU		0,PD0
PD0_1		.BTEQU		1,PD0
PD0_2		.BTEQU		2,PD0
PD0_3		.BTEQU		3,PD0
PD0_4		.BTEQU		4,PD0
PD0_5		.BTEQU		5,PD0
PD0_6		.BTEQU		6,PD0
PD0_7		.BTEQU		7,PD0
;
PD1			.EQU		03E3H
PD1_0		.BTEQU		0,PD1
PD1_1		.BTEQU		1,PD1
PD1_2		.BTEQU		2,PD1
PD1_3		.BTEQU		3,PD1
PD1_4		.BTEQU		4,PD1
PD1_5		.BTEQU		5,PD1
PD1_6		.BTEQU		6,PD1
PD1_7		.BTEQU		7,PD1
;
P2			.EQU		03E4H
P2_0		.BTEQU		0,P2
P2_1		.BTEQU		1,P2
P2_2		.BTEQU		2,P2
P2_3		.BTEQU		3,P2
P2_4		.BTEQU		4,P2
P2_5		.BTEQU		5,P2
P2_6		.BTEQU		6,P2
P2_7		.BTEQU		7,P2
;
P3			.EQU		03E5H
P3_0		.BTEQU		0,P3
P3_1		.BTEQU		1,P3
P3_2		.BTEQU		2,P3
P3_3		.BTEQU		3,P3
P3_4		.BTEQU		4,P3
P3_5		.BTEQU		5,P3
P3_6		.BTEQU		6,P3
P3_7		.BTEQU		7,P3
;
PD2			.EQU		03E6H
PD2_0		.BTEQU		0,PD2
PD2_1		.BTEQU		1,PD2
PD2_2		.BTEQU		2,PD2
PD2_3		.BTEQU		3,PD2
PD2_4		.BTEQU		4,PD2
PD2_5		.BTEQU		5,PD2
PD2_6		.BTEQU		6,PD2
PD2_7		.BTEQU		7,PD2
;
PD3			.EQU		03E7H
PD3_0		.BTEQU		0,PD3
PD3_1		.BTEQU		1,PD3
PD3_2		.BTEQU		2,PD3
PD3_3		.BTEQU		3,PD3
PD3_4		.BTEQU		4,PD3
PD3_5		.BTEQU		5,PD3
PD3_6		.BTEQU		6,PD3
PD3_7		.BTEQU		7,PD3
;
P4			.EQU		03E8H
P4_0		.BTEQU		0,P4
P4_1		.BTEQU		1,P4
P4_2		.BTEQU		2,P4
P4_3		.BTEQU		3,P4
P4_4		.BTEQU		4,P4
P4_5		.BTEQU		5,P4
P4_6		.BTEQU		6,P4
P4_7		.BTEQU		7,P4
;
P5			.EQU		03E9H
P5_0		.BTEQU		0,P5
P5_1		.BTEQU		1,P5
P5_2		.BTEQU		2,P5
P5_3		.BTEQU		3,P5
P5_4		.BTEQU		4,P5
P5_5		.BTEQU		5,P5
P5_6		.BTEQU		6,P5
P5_7		.BTEQU		7,P5
;
PD4			.EQU		03EAH
PD4_0		.BTEQU		0,PD4
PD4_1		.BTEQU		1,PD4
PD4_2		.BTEQU		2,PD4
PD4_3		.BTEQU		3,PD4
PD4_4		.BTEQU		4,PD4
PD4_5		.BTEQU		5,PD4
PD4_6		.BTEQU		6,PD4
PD4_7		.BTEQU		7,PD4
;
PD5			.EQU		03EBH
PD5_0		.BTEQU		0,PD5
PD5_1		.BTEQU		1,PD5
PD5_2		.BTEQU		2,PD5
PD5_3		.BTEQU		3,PD5
PD5_4		.BTEQU		4,PD5
PD5_5		.BTEQU		5,PD5
PD5_6		.BTEQU		6,PD5
PD5_7		.BTEQU		7,PD5
;
P6			.EQU		03ECH
P6_0		.BTEQU		0,P6
P6_1		.BTEQU		1,P6
P6_2		.BTEQU		2,P6
P6_3		.BTEQU		3,P6
P6_4		.BTEQU		4,P6
P6_5		.BTEQU		5,P6
P6_6		.BTEQU		6,P6
P6_7		.BTEQU		7,P6
;
P7			.EQU		03EDH
P7_0		.BTEQU		0,P7
P7_1		.BTEQU		1,P7
P7_2		.BTEQU		2,P7
P7_3		.BTEQU		3,P7
P7_4		.BTEQU		4,P7
P7_5		.BTEQU		5,P7
P7_6		.BTEQU		6,P7
P7_7		.BTEQU		7,P7
;
PD6			.EQU		03EEH
PD6_0		.BTEQU		0,PD6
PD6_1		.BTEQU		1,PD6
PD6_2		.BTEQU		2,PD6
PD6_3		.BTEQU		3,PD6
PD6_4		.BTEQU		4,PD6
PD6_5		.BTEQU		5,PD6
PD6_6		.BTEQU		6,PD6
PD6_7		.BTEQU		7,PD6
;
PD7			.EQU		03EFH
PD7_0		.BTEQU		0,PD7
PD7_1		.BTEQU		1,PD7
PD7_2		.BTEQU		2,PD7
PD7_3		.BTEQU		3,PD7
PD7_4		.BTEQU		4,PD7
PD7_5		.BTEQU		5,PD7
PD7_6		.BTEQU		6,PD7
PD7_7		.BTEQU		7,PD7
;
P8			.EQU		03F0H
P8_0		.BTEQU		0,P8
P8_1		.BTEQU		1,P8
P8_2		.BTEQU		2,P8
P8_3		.BTEQU		3,P8
P8_4		.BTEQU		4,P8
P8_5		.BTEQU		5,P8
P8_6		.BTEQU		6,P8
P8_7		.BTEQU		7,P8
;
P9			.EQU		03F1H
P9_0		.BTEQU		0,P9
P9_1		.BTEQU		1,P9
P9_2		.BTEQU		2,P9
P9_3		.BTEQU		3,P9
P9_4		.BTEQU		4,P9
P9_5		.BTEQU		5,P9
P9_6		.BTEQU		6,P9
P9_7		.BTEQU		7,P9
;
PD8			.EQU		03F2H
PD8_0		.BTEQU		0,PD8
PD8_1		.BTEQU		1,PD8
PD8_2		.BTEQU		2,PD8
PD8_3		.BTEQU		3,PD8
PD8_4		.BTEQU		4,PD8
PD8_5		.BTEQU		5,PD8
PD8_6		.BTEQU		6,PD8
PD8_7		.BTEQU		7,PD8
;
PD9			.EQU		03F3H
PD9_0		.BTEQU		0,PD9
PD9_1		.BTEQU		1,PD9
PD9_2		.BTEQU		2,PD9
PD9_3		.BTEQU		3,PD9
PD9_4		.BTEQU		4,PD9
PD9_5		.BTEQU		5,PD9
PD9_6		.BTEQU		6,PD9
PD9_7		.BTEQU		7,PD9
;
P10			.EQU		03F4H
P10_0		.BTEQU		0,P10
P10_1		.BTEQU		1,P10
P10_2		.BTEQU		2,P10
P10_3		.BTEQU		3,P10
P10_4		.BTEQU		4,P10
P10_5		.BTEQU		5,P10
P10_6		.BTEQU		6,P10
P10_7		.BTEQU		7,P10
;
PD10		.EQU		03F6H
PD10_0		.BTEQU		0,PD10
PD10_1		.BTEQU		1,PD10
PD10_2		.BTEQU		2,PD10
PD10_3		.BTEQU		3,PD10
PD10_4		.BTEQU		4,PD10
PD10_5		.BTEQU		5,PD10
PD10_6		.BTEQU		6,PD10
PD10_7		.BTEQU		7,PD10
;
;----------------------------------------------------------------
;	Pull up control register									 
;----------------------------------------------------------------
PUR0		.EQU		03FCH
;
PU00		.BTEQU		0,PUR0
PU01		.BTEQU		1,PUR0
PU02		.BTEQU		2,PUR0
PU03		.BTEQU		3,PUR0
PU04		.BTEQU		4,PUR0
PU05		.BTEQU		5,PUR0
PU06		.BTEQU		6,PUR0
PU07		.BTEQU		7,PUR0
;
PUR1		.EQU		03FDH
;
PU10		.BTEQU		0,PUR1
PU11		.BTEQU		1,PUR1
PU12		.BTEQU		2,PUR1
PU13		.BTEQU		3,PUR1
PU14		.BTEQU		4,PUR1
PU15		.BTEQU		5,PUR1
PU16		.BTEQU		6,PUR1
PU17		.BTEQU		7,PUR1
;
PUR2		.EQU		03FEH
;
PU20		.BTEQU		0,PUR2
PU21		.BTEQU		1,PUR2
PU22		.BTEQU		2,PUR2
PU23		.BTEQU		3,PUR2
PU24		.BTEQU		4,PUR2
PU25		.BTEQU		5,PUR2
