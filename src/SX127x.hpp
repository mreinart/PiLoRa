#ifndef __SX127x_HPP__
#define __SX127x_HPP__

namespace flylora_sx127x
{

// RegFifo      0x00
constexpr uint8_t REGFIFO                   = 0x0;

// RegOpMode    0x01
constexpr uint8_t REGOPMODE                 = 0x01;
constexpr uint8_t LONGRANGEMODEMASK         = 0b10000000;
constexpr uint8_t ACCESSSHAREDREGMASK       = 0b01000000; // AccessSharedReg
constexpr uint8_t LOWFREQUENCYMODEONMASK    = 0b00001000; // LowFrequencyModeOn
constexpr uint8_t MODEMASK                  = 0b00000111; // Mode

enum class AccessSharedReg            // This bit operates when device is in Lora mode; if set it allows access to FSK registers page located in address space (0x0D:0x3F) while in LoRa mode.
{
    LORA,                       // ccess LoRa registers page 0x0D:0x3F
    FSK                         // Access FSK registers page (in mode LoRa) 0x0D:0x3F
};

enum class Mode                       // Device modes
{
    SLEEP,                      // Sleep
    STDBY,                      // Standby
    FSTX,                       // Frequency synthesis TX
    TX,                         // TX
    FSRX,                       // Frequency synthesis RX
    RXCONTINUOUS,               // Receive continuous
    RXSINGLE,                   // Receive single
    CAD                         // Channel activity detection
};

inline const char* enumToString(Mode pVal)
{
    switch (pVal)
    {
        case Mode::SLEEP:
            return "SLEEP";
        case Mode::STDBY:
            return "STDBY";
        case Mode::FSTX:
            return "FSTX";
        case Mode::TX:
            return "TX";
        case Mode::FSRX:
            return "FSRX";
        case Mode::RXCONTINUOUS:
            return "RXCONTINUOUS";
        case Mode::RXSINGLE:
            return "RXSINGLE";
        case Mode::CAD:
            return "CAD";
        default:
            return "INVALID!";
    }
}

// RegFrMsb     0x06
constexpr uint8_t REGFRMSB                  = 0x06; // MSB (23:16) of RF carrier frequency
// RegFrMid     0x07
constexpr uint8_t REGFRMID                  = 0x07; // MSB (15:8) of RF carrier frequency
// RegFrLsb     0x08
constexpr uint8_t REGFRLSB                  = 0x08; // MSB (7:0) of RF carrier frequency

inline uint64_t convertRegFrToFrequency(uint32_t regfr, uint64_t fxosc) {return fxosc*regfr/524288;}

// RegPaConfig  0x09
constexpr uint8_t REGPACONFIG               = 0x09;
constexpr uint8_t PASELECTMASK              = 0b10000000; // PaSelect
constexpr uint8_t MAXPOWERMASK              = 0b01110000; // MaxPower
constexpr uint8_t OUTPUTPOWERMASK           = 0b00001111; // OutputPower

enum class PaSelect                   // Selects PA output pin
{
    RFO,                        // RFO pin. Output power is limited to +14 dBm.
    PA_BOOST                    // PA_BOOST pin. Output power is limited to +20 dBm.
};

inline double converMaxPowerToPmax(uint8_t maxPower) {return 10.8+0.6*maxPower;}

inline double convertOutputPowerToPout(bool isBoosted, double pmax, uint8_t outputPower)
{
    if (isBoosted)
        return pmax - 15 + outputPower;
    return 2 + outputPower;
}

// RegPaRamp    0x0A
constexpr uint8_t REGPARAMP                 = 0x0A;
constexpr uint8_t PARAMPMASK                = 0b00001111; // PaRamp

enum class PaRamp                     // Rise/Fall time of ramp up/down in FS
{
    RAMP_3P4_MS,                   // 3.4 ms
    RAMP_2_MS,                     // 2 ms
    RAMP_1_MS,                     // 1 ms
    RAMP_500_US,                   // 500 us
    RAMP_250_US,                   // 250 us
    RAMP_125_US,                   // 125 us
    RAMP_100_US,                   // 100 us
    RAMP_62_US,                    // 62 us
    RAMP_50_US,                    // 50 us
    RAMP_40_US,                    // 40 us
    RAMP_31_US,                    // 31 us
    RAMP_25_US,                    // 25 us
    RAMP_20_US,                    // 20 us
    RAMP_15_US,                    // 15 us
    RAMP_12_US,                    // 12 us
    RAMP_10_US,                    // 10 us
};

// RegPaRamp    0x0B
constexpr uint8_t REGOCP                 = 0x0B;
constexpr uint8_t OCPONMASK                 = 0b00100000; // OcpOn
constexpr uint8_t OCPTRIMMASK               = 0b00011111; // OcpTrim

inline double convertOcpTrimToImax(uint8_t ocpTrim)
{
    if (ocpTrim <= 15)
        return 45+5*ocpTrim;
    return -30+10*ocpTrim;
}

// RegLna       0x0C
constexpr uint8_t REGLNA                    = 0x0C;
constexpr uint8_t LNAGAINMASK               = 0b11100000; // LnaGain
constexpr uint8_t LNABOOSTLFMASK            = 0b00011000; // LnaBoostLf
constexpr uint8_t LNABOOSTHFMASK            = 0b00000011; // LnaBoostHf

enum class LnaGain                    // LNA Gain Setting
{
    G1 = 1,                     // G1 = maximum gain
    G2,                         // G2
    G3,                         // G3
    G4,                         // G4
    G5,                         // G5
    G6                          // G6 = minimum gain
};

enum class LnaBoostLf                 // Low Frequency (RFI_LF) LNA current adjustment
{
    DEFAULT                     // Default LNA current
};

enum class LnaBoostHf                 // High Frequency (RFI_HF) LNA current adjustment
{
    DEFAULT,                    // Default LNA current
    BOOST_ON                    // Boost on, 150% LNA current
};


// RegFifoAddrPtr               0x0D
constexpr uint8_t REGFIFOADDRPTR            = 0x0D; // SPI interface address pointer in FIFO data buffer
// RegFifoTxBaseAdd             0x0E
constexpr uint8_t REGFIFOTXBASEADD          = 0x0E; // Write base address in FIFO data buffer for TX modulator
// RegFifoRxBaseAdd             0x0F
constexpr uint8_t REGFIFORXBASEADD          = 0x0F; // Read base address in FIFO data buffer for RX demodulator
// RegFifoRxCurrentAddr         0x10
constexpr uint8_t REGFIFORXCURRENTADDR      = 0x10; // Start address (in data buffer) of last packet received

// RegIrqFlagsMaskMask          0x11
constexpr uint8_t REGIRQFLAGSMASK           = 0x11;
constexpr uint8_t RXTIMEOUTMASKMASK         = 0b10000000; // Timeout interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t RXDONEMASKMASK            = 0b01000000; // Packet reception complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t PAYLOADCRCERRORMASKMASK   = 0b00100000; // Payload CRC error interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t VALIDHEADERMASKMASK       = 0b00010000; // Valid header received in Rx mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t TXDONEMASKMASK            = 0b00001000; // FIFO Payload transmission complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t CADDONEMASKMASK           = 0b00000100; // CAD complete interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t FHSSCHANGECHANNELMASKMASK = 0b00000010; // FHSS change channel interrupt mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t CADDETECTEDMASKMASK       = 0b00000001; // Cad Detected Interrupt Mask: setting this bit masks the corresponding IRQ in RegIrqFlags

// RegIrqFlags                  0x12
constexpr uint8_t REGIRQFLAGS               = 0x12;
constexpr uint8_t RXTIMEOUTMASK             = 0b10000000; // Timeout interrupt: writing a 1 clears the IRQ
constexpr uint8_t RXDONEMASK                = 0b01000000; // Packet reception complete interrupt: writing a 1 clears the IRQ
constexpr uint8_t PAYLOADCRCERRORMASK       = 0b00100000; // Payload CRC error interrupt: writing a 1 clears the IRQ
constexpr uint8_t VALIDHEADERMASK           = 0b00010000; // Valid header received in Rx mask: setting this bit masks the corresponding IRQ in RegIrqFlags
constexpr uint8_t TXDONEMASK                = 0b00001000; // FIFO Payload transmission complete interrupt: writing a 1 clears the IRQ
constexpr uint8_t CADDONEMASK               = 0b00000100; // CAD complete: write to clear: writing a 1 clears the IRQ
constexpr uint8_t FHSSCHANGECHANNELMASK     = 0b00000010; // FHSS change channel interrupt: writing a 1 clears the IRQ
constexpr uint8_t CADDETECTEDMASK           = 0b00000001; // Valid Lora signal detected during CAD operation: writing a 1 clears the IRQ

// RegRxNbBytes                 0x13
constexpr uint8_t REGRXNBBYTES              = 0x13; // Number of payload bytes of latest packet received

// RegRxHeaderCntValueMsb       0x14
constexpr uint8_t REGRXHEADERCNTVALUEMSB    = 0x14; // Number of valid headers received since last transition into Rx mode, MSB(15:8). Header and packet counters are reseted in Sleep mode.
// RegRxHeaderCntValueLsb       0x15
constexpr uint8_t REGRXHEADERCNTVALUELSB    = 0x15; // Number of valid headers received since last transition into Rx mode, LSB(7:0). Header and packet counters are reseted in Sleep mode.

// RegRxPacketCntValueMsb       0x16
constexpr uint8_t REGRXPACKETCNTVALUEMSB    = 0x16; // Number of valid packets received since last transition into Rx mode, MSB(15:8). Header and packet counters are reseted in Sleep mode.
// RegRxPacketCntValueLsb       0x17
constexpr uint8_t REGRXPACKETCNTVALUELSB    = 0x17; // Number of valid packets received since last transition into Rx mode, LSB(7:0). Header and packet counters are reseted in Sleep mode.

// RegModemStat                 0x18
constexpr uint8_t REGMODEMSTAT              = 0x18;
constexpr uint8_t RXCODINGRATEMASK          = 0b11100000; // Coding rate of last header received
constexpr uint8_t MODEMCLEARMASK            = 0b00010000; // Modem Clear
constexpr uint8_t HEADERINFOVALIDMASK       = 0b00001000; // Header Info Valid
constexpr uint8_t RXONGOINGMASK             = 0b00000100; // RxOnGoingMask
constexpr uint8_t SIGNALSYNCHRONIZEDMASK    = 0b00000010; // Signal Synchronized
constexpr uint8_t SIGNALDETECTEDMASK        = 0b00000001; // Signal Detected

// RegPktSnrValue               0x19
constexpr uint8_t REGPKTSNRVALUE            = 0x19;

inline double convertPktSnrValueToSNR(uint8_t pktSnr) {return -pktSnr/4;}

// RegPktRssiValue              0x1A
constexpr uint8_t REGPKTRSSIVALUE           = 0x1A; // RSSI of the latest packet received (dBm)

inline double convertPacketRssiValueToRssi(bool isHF, uint8_t pktRssi)
{
    if (isHF)
        return -157 + int(pktRssi);
    return -164 + int(pktRssi);
}

// RegRssiValue                 0x1B
constexpr uint8_t REGRSSIVALUE              = 0x1B; // Current RSSI value (dBm)

inline double convertRssiValueToRssi(bool isHF, uint8_t rssi)
{
    if (isHF)
        return -157 + int(rssi);
    return -164 + int(rssi);
}

// RegHopChannel                0x1C
constexpr uint8_t REGHOPCHANNEL             = 0x1C;
constexpr uint8_t PLLTIMEOUTMASK            = 0b00000001; // PLL failed to lock while attempting a TX/RX/CAD operation
constexpr uint8_t CRCONPAYLOADMASK          = 0b00000001; // CRC Information extracted from the received packet header (Explicit header mode only)
constexpr uint8_t FHSSPRESENTCHANNELMASK    = 0b00011111; // Current value of frequency hopping channel in use


// RegModemConfig1              0x1D
constexpr uint8_t REGMODEMCONFIG1           = 0x1D;
constexpr uint8_t BWMASK                    = 0b11110000; // Bw
constexpr uint8_t CODINGRATEMASK            = 0b00001110; // CodingRate
constexpr uint8_t IMPLICITHEADERMODEONMASK  = 0b00000001; // ImplicitHeaderModeOn

enum class Bw
{
    BW_7P8_KHZ,                  // 7.8 KHZ
    BW_10P4_KHZ,                 // 10.4 KHZ
    BW_15P6_KHZ,                 // 15.6 KHZ
    BW_20P8_KHZ,                 // 20.8 KHZ
    BW_31P25_KHZ,                // 31.25 KHZ
    BW_41P7_KHZ,                 // 41.7 KHZ
    BW_62P5_KHZ,                 // 62.5 KHZ
    BW_125_KHZ,                  // 125 KHZ
    BW_250_KHZ,                  // 250 KHZ
    BW_500_KHZ,                  // 500 KHZ
};

enum class CodingRate
{
    CR_4V5 = 1,                  // 4/5
    CR_4V6,                      // 4/6
    CR_4V7,                      // 4/7
    CR_4V8                       // 4/8
};

// RegModemConfig2              0x1E
constexpr uint8_t REGMODEMCONFIG2           = 0x1E;
constexpr uint8_t SPREADINGFACTORMASK        = 0b11110000; // SpreadingFactor
constexpr uint8_t TXCONTINOUSMODEMASK       = 0b00001000; // TxContinousMode
constexpr uint8_t RXPAYLOADCRCONMASK        = 0b00000100; // RxPayloadCrcOn
constexpr uint8_t SYMBTIMEOUTMSBMASK        = 0b00000011; // SymbTimeout MSB

enum class SpreadingFactor
{
    SF_6 = 6,                   // SF6
    SF_7,                       // SF7
    SF_8,                       // SF8
    SF_9,                       // SF9
    SF_10,                      // SF10
    SF_11,                      // SF11
    SF_12                       // SF12
};

// RegSymbTimeoutLsb            0x1F
constexpr uint8_t REGSYMBTIMEOUTLSB         = 0x1F; // SymbTimeout MSB

inline double convertSymbTimeoutToTimeout(double ts, uint16_t symbTimeout)
{
    return ts * symbTimeout;
}

// RegPreambleMsb               0x20
constexpr uint8_t REGPREAMBLEMSB            = 0x20; // Preamble Length MSB
// RegPreambleLsb               0x21
constexpr uint8_t REGPREAMBLELSB            = 0x21; // Preamble Length LSB
// RegPayloadLength             0x22
constexpr uint8_t REGPAYLOADLENGTH          = 0x22; // Payload Length
// RegMaxPayloadLength          0x23
constexpr uint8_t REGMAXPAYLOADLENGTH       = 0x23; // Max Payload Length
// RegHopPeriod                 0x24
constexpr uint8_t REGHOPPERIOD              = 0x24; // Frequency Hopping Period
// RegFifoRxByteAddr            0x25
constexpr uint8_t REGFIFORXBYTEADDR         = 0x25; // Current value of RX data buffer pointe


// RegModemConfig3              0x26
constexpr uint8_t REGMODEMCONFIG3           = 0x26;
constexpr uint8_t LOWDATARATEOPTIMIZEMASK   = 0b00001000; // LowDataRateOptimize
constexpr uint8_t AGCAUTOONMASK             = 0b00000100; // AgcAutoOn

// RegPpmCorrection             0x27
constexpr uint8_t REGPPMCORRECTION          = 0x27; // Data rate offset value, used in conjunction with AFC

// RegFeiMsb                    0x28
constexpr uint8_t REGFEIMSB                 = 0x28;
constexpr uint8_t FEIMSBMASK                = 0b00001111; // FrequencyErrpr MSB

// RegFeiMid                    0x29
constexpr uint8_t REGFEIMID                 = 0x29; // FrequencyErrpr MID
// RegFeiLsb                    0x2A
constexpr uint8_t REGFEILSB                 = 0x2A; // FrequencyErrpr LSB

inline double convertFreqErrorToFError(uint32_t freqError, uint32_t bw, uint32_t fxtal) {return (freqError*16777216*bw)/(fxtal*500);}

// RegRssiWideband              0x2C
constexpr uint8_t REGRSSIWIDEBAND           = 0x2C; // Wideband RSSI measurement used to locally generate a random number

// RegDetectOptimize            0x31
constexpr uint8_t REGDETECTOPTIMIZE         = 0x31;
constexpr uint8_t DETECTOPTIMIZEMASK        = 0x00000011;

enum class DetectOptimize             // LoRa Detection Optimize
{
    SF_6 = 5,                   // SF6
    SF_7 = 3,                   // SF7
    SF_8 = 3,                   // SF8
    SF_9 = 3,                   // SF9
    SF_10 = 3,                  // SF10
    SF_11 = 3,                  // SF11
    SF_12 = 3                   // SF12
};

// RegInvertIQ                  0x33
constexpr uint8_t REGINVERTIQ               = 0x33;
constexpr uint8_t INVERTIQMASK              = 0b01000000; // Invert the LoRa I and Q signals


// RegDetectionThreshold        0x37
constexpr uint8_t REGDETECTIONTHRESHOLD     = 0x37;

enum class DetectionThreshold         // LoRa detection threshold
{
    SF_6 = 0xC,                 // SF6
    SF_7 = 0xA,                 // SF7
    SF_8 = 0xA,                 // SF8
    SF_9 = 0xA,                 // SF9
    SF_10 = 0xA,                // SF10
    SF_11 = 0xA,                // SF11
    SF_12 = 0xA                 // SF12
};

// RegSyncWord                  0x39
constexpr uint8_t REGSYNCWORD               = 0x39; // LoRa Sync Word

// RegDioMapping1               0x40
constexpr uint8_t REGDIOMAPPING1            = 0x40; // DIO Mapping for DIO3:DIO0

// RegDioMapping2               0x41
constexpr uint8_t REGDIOMAPPING2            = 0x41; // DIO Mapping for DIO5:DIO4

//                                                           D0D1D2D3
constexpr uint8_t DIO0RXDONEMASK            = 0b00000000; // 00------
constexpr uint8_t DIO0TXDONEMASK            = 0b01000000; // 01------
constexpr uint8_t DIO0CADDONEMASK           = 0b10000000; // 10------
constexpr uint8_t DIO0NOPMASK               = 0b10000000; // 11------
constexpr uint8_t DIO1RXTOUTMASK            = 0b00000000; // --00----
constexpr uint8_t DIO1FHSSCHANGECHANNELMASK = 0b00010000; // --01----
constexpr uint8_t DIO1CADDETECTEDMASK       = 0b00100000; // --10----
constexpr uint8_t DIO1NOPMASK               = 0b00110000; // --11----
constexpr uint8_t DIO2FHSSCHANGECHANNELMASK = 0b00000000; // ----00--
constexpr uint8_t DIO2NOPMASK               = 0b00001100; // ----11--
constexpr uint8_t DIO3CADDONEMASK           = 0b00000000; // ------00
constexpr uint8_t DIO3VALIDHEADERMASK       = 0b00000001; // ------01
constexpr uint8_t DIO3PAYLOADCRCMASK        = 0b00000010; // ------10
constexpr uint8_t DIO3NOPMASK               = 0b00000011; // ------11

enum class DioMapping2
{
    ModeReady_CadDetected,
    ClkOut_PllLock,
    ClkOut_PllLock_2,
};

// RegVersion                   0x42
constexpr uint8_t REGVERSION                = 0x42; // Version

// RegPllHop                    0x44
constexpr uint8_t REGPLLHOP                 = 0x44;
constexpr uint8_t FASTHOPONMASK             = 0b10000000; // FastHopOn

// RegTcxo                      0x4B
constexpr uint8_t REGTCXO                   = 0x4B;
constexpr uint8_t TCXOINPUTONMASK           = 0b00001000;

// RegPaDac                     0x4D
constexpr uint8_t REGPADAC                  = 0x4D;
constexpr uint8_t PADACMASK                 = 0b00000011; // PaDac
enum class PaDac
{
    DEFAULT = 4,
    PA_BOOST = 7
};

// RegFormerTemp                0x5B
constexpr uint8_t REGFORMERTEMP             = 0x5B; // LoRa Sync Word

// RegAgcRef                    0x61
constexpr uint8_t REGAGCREF                 = 0x61;
constexpr uint8_t AGCREFERENCELEVELMASK     = 0b00111111;

// RegAgcThresh1                0x62
constexpr uint8_t REGAGCTHRESH1             = 0x62; // Defines the 1st AGC Threshold
// RegAgcThresh2                0x63
constexpr uint8_t REGAGCTHRESH2             = 0x63; // Defines the 2nd AGC Threshold
// RegAgcThresh3                0x64
constexpr uint8_t REGAGCTHRESH3             = 0x64; // Defines the 3rd AGC Threshold

//RegPll                        0x70
constexpr uint8_t REGPLL                    = 0x70;
constexpr uint8_t PLLBANDWIDTHMASK          = 0b11000000;
enum class PllBandwidth
{
    E_76KHZ,
    E_150KHZ,
    E_255KHZ,
    E_300KHZ
};

inline const char* regIndexToString(uint8_t pReg)
{
    switch (pReg)
    {
        case 0x00:
            return "RegFifo";
        case 0x01:
            return "RegOpMode";
        case 0x06:
            return "RegFrMsb";
        case 0x07:
            return "RegFrMid";
        case 0x08:
            return "RegFrLsb";
        case 0x09:
            return "RegPaConfig";
        case 0x0A:
            return "RegPaRamp";
        case 0x0B:
            return "RegPaRamp";
        case 0x0C:
            return "RegLna";
        case 0x0D:
            return "RegFifoAddrPtr";
        case 0x0E:
            return "RegFifoTxBaseAdd";
        case 0x0F:
            return "RegFifoRxBaseAdd";
        case 0x10:
            return "RegFifoRxCurrentAddr";
        case 0x11:
            return "RegIrqFlagsMaskMask";
        case 0x12:
            return "RegIrqFlags";
        case 0x13:
            return "RegRxNbBytes";
        case 0x14:
            return "RegRxHeaderCntValueMsb";
        case 0x15:
            return "RegRxHeaderCntValueLsb";
        case 0x16:
            return "RegRxPacketCntValueMsb";
        case 0x17:
            return "RegRxPacketCntValueLsb";
        case 0x18:
            return "RegModemStat";
        case 0x19:
            return "RegPktSnrValue";
        case 0x1A:
            return "RegPktRssiValue";
        case 0x1B:
            return "RegRssiValue";
        case 0x1C:
            return "RegHopChannel";
        case 0x1D:
            return "RegModemConfig1";
        case 0x1E:
            return "RegModemConfig2";
        case 0x1F:
            return "RegSymbTimeoutLsb";
        case 0x20:
            return "RegPreambleMsb";
        case 0x21:
            return "RegPreambleLsb";
        case 0x22:
            return "RegPayloadLength";
        case 0x23:
            return "RegMaxPayloadLength";
        case 0x24:
            return "RegHopPeriod";
        case 0x25:
            return "RegFifoRxByteAddr";
        case 0x26:
            return "RegModemConfig3";
        case 0x27:
            return "RegPpmCorrection";
        case 0x28:
            return "RegFeiMsb";
        case 0x29:
            return "RegFeiMid";
        case 0x2A:
            return "RegFeiLsb";
        case 0x2C:
            return "RegRssiWideband";
        case 0x31:
            return "RegDetectOptimize";
        case 0x33:
            return "RegInvertIQ";
        case 0x37:
            return "RegDetectionThreshold";
        case 0x39:
            return "RegSyncWord";
        case 0x40:
            return "RegDioMapping1";
        case 0x41:
            return "RegDioMapping2";
        case 0x42:
            return "RegVersion";
        case 0x44:
            return "RegPllHop";
        case 0x4B:
            return "RegTcxo";
        case 0x4D:
            return "RegPaDac";
        case 0x5B:
            return "RegFormerTemp";
        case 0x61:
            return "RegAgcRef";
        case 0x62:
            return "RegAgcThresh1";
        case 0x63:
            return "RegAgcThresh2";
        case 0x64:
            return "RegAgcThresh3";
        default:
            return "invalid";
    }
}

inline uint64_t getUnmasked(uint64_t mask, uint64_t value)
{
    for (; !(mask&1); mask = (mask>>1), value = (value>>1));
    return mask&value;
}

inline uint64_t setMasked(uint64_t mask, uint64_t value)
{
    auto omask = mask;
    for (; !(mask&1); mask = (mask>>1), value = (value<<1));
    return value&omask;
}

}

#endif // __SX127x_HPP__