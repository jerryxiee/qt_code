void TP2827C_StartTX(unsigned char chip, unsigned char ch)
{
	unsigned char tmp;
	int i;

	//tp28xx_byte_write(chip, 0xB6, (0x01<<(ch))); //clear flag

	tmp = tp28xx_byte_read(chip, 0x6f);
    tmp |= 0x01;
    tp28xx_byte_write(chip, 0x6f, tmp); //TX enable

    i = 100;
    while(i--)
    {
        if( 0x00 == tp28xx_byte_read(chip, 0x6d)) break;
        //udelay(1000);
        msleep(2);
    }

    tmp &= 0xfe;
    tp28xx_byte_write(chip, 0x6f, tmp); //TX disable
}
static void TP2827C_RX_init(unsigned char chip, unsigned char mod)
{

    int i, index=0;

    static const unsigned char PTZ_RX_dat[][15]=
    {
        {0x00,0x00,0x07,0x08,0x00,0x00,0x04,0x00,0x00,0x60,0x10,0x04,0xf0,0xd8,0x27}, //TVI command
        {0x00,0x00,0x07,0x08,0x09,0x0a,0x04,0x00,0x00,0x60,0x10,0x04,0xf0,0xd8,0x27}, //TVI burst
        {0x00,0x00,0x06,0x07,0x08,0x09,0x03,0x52,0x53,0x60,0x10,0x04,0xf0,0xd8,0x17}, //ACP1 0.525
        {0x00,0x00,0x06,0x07,0x08,0x09,0x02,0xdf,0x88,0x60,0x10,0x04,0xf0,0xd8,0x17}, //ACP2 0.6
        {0x00,0x00,0x06,0x07,0x08,0x09,0x04,0xec,0xe9,0x60,0x10,0x04,0xf0,0xd8,0x17}, //ACP3 0.35
    };

        if(PTZ_RX_TVI_CMD == mod)
        {
            index = 0;
        }
        else if(PTZ_RX_TVI_BURST == mod)
        {
            index = 1;
        }
        else if(PTZ_RX_ACP1 == mod)
        {
            index = 2;
        }
        else if(PTZ_RX_ACP2 == mod)
        {
            index = 3;
        }
        else if(PTZ_RX_ACP3 == mod)
        {
            index = 4;
        }

        for(i = 0; i < 15; i++)
        {
            tp28xx_byte_write(chip, 0xc9+i, PTZ_RX_dat[index][i]);
        }

}
static void TP2827C_PTZ_mode(unsigned char chip, unsigned char ch, unsigned char mod)
{
    unsigned int tmp, i, index=0;

    static const unsigned char PTZ_reg[12]=
    {
        0x6f,0x70,0x71,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8
    };
    static const unsigned char PTZ_dat[][12]=
    {
        {0x42,0x40,0x00,0x00,0x00,0x0b,0x0c,0x0d,0x0e,0x19,0x78,0x21}, //TVI1.0
        {0x42,0x40,0x00,0x00,0x00,0x0b,0x0c,0x0d,0x0e,0x33,0xf0,0x21}, //TVI2.0
        {0x42,0x40,0x00,0x00,0x00,0x0e,0x0f,0x10,0x11,0x26,0xf0,0x57}, //A1080p for 2826 0.525
        {0x42,0x40,0x00,0x00,0x00,0x0e,0x0f,0x00,0x00,0x26,0xe0,0xef}, //A720p for 2826 0.525
        {0x42,0x40,0x00,0x00,0x00,0x0f,0x10,0x00,0x00,0x4d,0xf0,0x6f}, //960H for 2826
        //{0x4a,0x5d,0x00,0x00,0x00,0x10,0x11,0x12,0x13,0x15,0xb8,0x9f}, //HDC for 2826
        {0x42,0x40,0x00,0x00,0x00,0x10,0x11,0x12,0x15,0x15,0xb8,0xa2}, //HDC for 2826
        {0x42,0x40,0x00,0x00,0x00,0x0f,0x10,0x11,0x12,0x2c,0xf0,0x57}, //ACP 3M18 for 2826 8+0.6
        {0x42,0x40,0x00,0x00,0x00,0x0f,0x10,0x11,0x12,0x19,0xd0,0x17}, //ACP 3M2530 for 2826 4+0.35
        {0x46,0x5f,0x00,0x00,0x00,0x0f,0x10,0x12,0x16,0x19,0xd0,0x17}, //ACP 4M2530_5M20 for 2826 4+0.35
        {0x46,0x5f,0x00,0x00,0x00,0x0f,0x10,0x12,0x16,0x2c,0xf0,0x57}, //ACP 4M15 5M12.5 for 2826 8+0.6
        {0x42,0x40,0x00,0x00,0x00,0x16,0x17,0x18,0x19,0x2a,0xf0,0x1a}, //HDC QHD25/30 for 2826
    };

    tp28xx_byte_write(chip, 0x40, ch); //reg bank1 switch for 2826

        if(PTZ_TVI == mod)
        {
            tmp = tp28xx_byte_read(chip, 0xf5); //check TVI 1 or 2
            if( (tmp >>ch) & 0x01)
            {
                index = 1;
            }
            else
            {
                index = 0;
            }
        }
        else if(PTZ_HDA_1080P == mod) //HDA 1080p
        {
                index = 2;
        }
        else if(PTZ_HDA_720P == mod) //HDA 720p
        {
                index = 3;
        }
        else if(PTZ_HDA_CVBS == mod) //HDA CVBS
        {
                index = 4;
        }
        else if(PTZ_HDC == mod) // test
        {
                index = 5;
        }
        else if(PTZ_HDA_3M18 == mod) // test
        {
                index = 6;
        }
        else if(PTZ_HDA_3M25 == mod) // test
        {
                index = 7;
        }
        else if(PTZ_HDA_4M25 == mod) // test
        {
                index = 8;
        }
        else if(PTZ_HDA_4M15 == mod) // test
        {
                index = 9;
        }
        else if(PTZ_HDC_QHD == mod) // test
        {
                index = 10;
        }

     for(i = 0; i < 12; i++)
     {
         tp28xx_byte_write(chip, PTZ_reg[i], PTZ_dat[index][i]);
     }

}

static void TP2827C_output(unsigned char chip)
{
    unsigned int tmp;

    tp28xx_byte_write(chip, 0xF5, 0x00);
    tp28xx_byte_write(chip, 0xF1, 0x00);
    //tp28xx_byte_write(chip, 0x4D, 0x0f);
    //tp28xx_byte_write(chip, 0x4E, 0x0f);
    tp28xx_byte_write(chip, 0x4f, 0x03);

    if( SDR_1CH == output[chip] )
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x88);
        tp28xx_byte_write(chip, 0xF6, 0x00);
        tp28xx_byte_write(chip, 0xF7, 0x11);
        tp28xx_byte_write(chip, 0xF8, 0x22);
        tp28xx_byte_write(chip, 0xF9, 0x33);
        tp28xx_byte_write(chip, 0x50, 0x00); //
        tp28xx_byte_write(chip, 0x51, 0x00); //
        tp28xx_byte_write(chip, 0x52, 0x00); //
        tp28xx_byte_write(chip, 0x53, 0x00); //
        tp28xx_byte_write(chip, 0xF3, 0x00);
        tp28xx_byte_write(chip, 0xF2, 0x00);
        if(TP2802_720P25V2 == mode || TP2802_720P30V2 == mode || TP2802_PAL == mode || TP2802_NTSC == mode )
        {
            tmp = tp28xx_byte_read(chip, 0xFA);
            tmp &= 0x88;
            tmp |= 0x11;
            tp28xx_byte_write(chip, 0xFA, tmp);
            tmp = tp28xx_byte_read(chip, 0xFB);
            tmp &= 0x88;
            tmp |= 0x11;
            tp28xx_byte_write(chip, 0xFB, tmp);
        }
        else if(FLAG_HALF_MODE == (mode & FLAG_HALF_MODE) )
        {
            tmp = tp28xx_byte_read(chip, 0xFA);
            tmp &= 0x88;
            tmp |= 0x43;
            tp28xx_byte_write(chip, 0xFA, tmp);
            tmp = tp28xx_byte_read(chip, 0xFB);
            tmp &= 0x88;
            tmp |= 0x65;
            tp28xx_byte_write(chip, 0xFB, tmp);
        }
    }
    else if(SDR_2CH == output[chip])
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x88);
        tp28xx_byte_write(chip, 0xF6, 0x10);
        tp28xx_byte_write(chip, 0xF7, 0x23);
        tp28xx_byte_write(chip, 0xF8, 0x10);
        tp28xx_byte_write(chip, 0xF9, 0x23);
        tp28xx_byte_write(chip, 0x50, 0x00); //
        tp28xx_byte_write(chip, 0x51, 0x00); //
        tp28xx_byte_write(chip, 0x52, 0x00); //
        tp28xx_byte_write(chip, 0x53, 0x00); //
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);
        tp28xx_byte_write(chip, 0xF4, 0x00); //output clock 148.5M

    }
    else if(DDR_2CH == output[chip])
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x88);
        //tp28xx_byte_write(chip, 0x45, 0x54); //PLL 297M
        tp28xx_byte_write(chip, 0xF4, 0xE0); //output clock 148.5M
        tp28xx_byte_write(chip, 0xF6, 0x10); //
        tp28xx_byte_write(chip, 0xF7, 0x23); //
        tp28xx_byte_write(chip, 0xF8, 0x10); //
        tp28xx_byte_write(chip, 0xF9, 0x23); //
        tp28xx_byte_write(chip, 0x50, 0x00); //
        tp28xx_byte_write(chip, 0x51, 0x00); //
        tp28xx_byte_write(chip, 0x52, 0x00); //
        tp28xx_byte_write(chip, 0x53, 0x00); //
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);

    }
    else if(DDR_4CH == output[chip])
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x88);
        //tp28xx_byte_write(chip, 0x45, 0x54); //PLL 297M
        tp28xx_byte_write(chip, 0xF4, 0xE0); //output clock 148.5M
        tp28xx_byte_write(chip, 0xF6, 0x10); //
        tp28xx_byte_write(chip, 0xF7, 0x10); //
        tp28xx_byte_write(chip, 0xF8, 0x10); //
        tp28xx_byte_write(chip, 0xF9, 0x10); //
        tp28xx_byte_write(chip, 0x50, 0xB2); //
        tp28xx_byte_write(chip, 0x51, 0xB2); //
        tp28xx_byte_write(chip, 0x52, 0xB2); //
        tp28xx_byte_write(chip, 0x53, 0xB2); //
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);
    }
    else if( DDR_1CH == output[chip] )
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x88);
        //tp28xx_byte_write(chip, 0x45, 0x54); //PLL 297M
        tp28xx_byte_write(chip, 0xF4, 0xE0); //output clock 148.5M
        tp28xx_byte_write(chip, 0xF6, 0x04);
        tp28xx_byte_write(chip, 0xF7, 0x15);
        tp28xx_byte_write(chip, 0xF8, 0x26);
        tp28xx_byte_write(chip, 0xF9, 0x37);
        tp28xx_byte_write(chip, 0x50, 0x00); //
        tp28xx_byte_write(chip, 0x51, 0x00); //
        tp28xx_byte_write(chip, 0x52, 0x00); //
        tp28xx_byte_write(chip, 0x53, 0x00); //
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);
    }
   // tp28xx_byte_write(chip, 0x4D, 0x0f);
   // tp28xx_byte_write(chip, 0x4E, 0x0f);
}

static void TP2827C_reset_default(unsigned char chip, unsigned char ch)
{
    unsigned int tmp;
    //tp28xx_byte_write(chip, 0x26, 0x04);
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x38, 0x00);
    tp28xx_byte_write(chip, 0x39, 0x1C);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3B, 0x26);

    tmp = tp28xx_byte_read(chip, 0x26);
    tmp &= 0xfe;
    tp28xx_byte_write(chip, 0x26, tmp);

    tmp = tp28xx_byte_read(chip, 0x06);
    tmp &= 0xfb;
    tp28xx_byte_write(chip, 0x06, tmp);

}
//////////////////////////////////////////////////////////////
static void TP2827C_NTSC_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x13);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x40);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0x2d);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x70);
    tp28xx_byte_write(chip, 0x2c, 0x2a);
    tp28xx_byte_write(chip, 0x2d, 0x68);
    tp28xx_byte_write(chip, 0x2e, 0x57);

    tp28xx_byte_write(chip, 0x30, 0x62);
    tp28xx_byte_write(chip, 0x31, 0xbb);
    tp28xx_byte_write(chip, 0x32, 0x96);
    tp28xx_byte_write(chip, 0x33, 0xc0);
    //tp28xx_byte_write(chip, 0x35, 0x25);
    tp28xx_byte_write(chip, 0x38, 0x00);
    tp28xx_byte_write(chip, 0x39, 0x04);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3B, 0x26);

    tp28xx_byte_write(chip, 0x18, 0x12);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
static void TP2827C_PAL_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x13);
    tp28xx_byte_write(chip, 0x0d, 0x51);

    tp28xx_byte_write(chip, 0x20, 0x48);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x37);
    tp28xx_byte_write(chip, 0x23, 0x3f);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0x2d);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x70);
    tp28xx_byte_write(chip, 0x2c, 0x2a);
    tp28xx_byte_write(chip, 0x2d, 0x64);
    tp28xx_byte_write(chip, 0x2e, 0x56);

    tp28xx_byte_write(chip, 0x30, 0x7a);
    tp28xx_byte_write(chip, 0x31, 0x4a);
    tp28xx_byte_write(chip, 0x32, 0x4d);
    tp28xx_byte_write(chip, 0x33, 0xf0);
    //tp28xx_byte_write(chip, 0x35, 0x25);
    tp28xx_byte_write(chip, 0x38, 0x00);
    tp28xx_byte_write(chip, 0x39, 0x04);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3B, 0x26);

    tp28xx_byte_write(chip, 0x18, 0x17);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
static void TP2827C_V1_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x30);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0x2d);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x30);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0xbb);
    tp28xx_byte_write(chip, 0x32, 0x2e);
    tp28xx_byte_write(chip, 0x33, 0x90);
    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x00);
    tp28xx_byte_write(chip, 0x39, 0x1c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3B, 0x26);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
static void TP2827C_V2_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x13);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x30);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0x2d);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x30);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0xbb);
    tp28xx_byte_write(chip, 0x32, 0x2e);
    tp28xx_byte_write(chip, 0x33, 0x90);
    //tp28xx_byte_write(chip, 0x35, 0x25);
    tp28xx_byte_write(chip, 0x38, 0x00);
    tp28xx_byte_write(chip, 0x39, 0x18);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3B, 0x26);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}

/////HDA QHD30
static void TP2827C_AQHDP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x16);
    tp28xx_byte_write(chip, 0x18, 0x32);

    tp28xx_byte_write(chip, 0x20, 0x80);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x36);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2d, 0xa0);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0x6a);
    tp28xx_byte_write(chip, 0x32, 0xbe);
    tp28xx_byte_write(chip, 0x33, 0x80);
    //tp28xx_byte_write(chip, 0x35, 0x15);
    tp28xx_byte_write(chip, 0x39, 0x60);
}

/////HDA QHD25
static void TP2827C_AQHDP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x16);
    tp28xx_byte_write(chip, 0x18, 0x32);

    tp28xx_byte_write(chip, 0x20, 0x80);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x36);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2d, 0xa0);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0x6f);
    tp28xx_byte_write(chip, 0x32, 0xb5);
    tp28xx_byte_write(chip, 0x33, 0x80);
    //tp28xx_byte_write(chip, 0x35, 0x15);
    tp28xx_byte_write(chip, 0x39, 0x60);
}

/////HDA QXGA30
static void TP2827C_AQXGAP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);

    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x90);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x01);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0xa0);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0x68);
    tp28xx_byte_write(chip, 0x32, 0xbe);
    tp28xx_byte_write(chip, 0x33, 0x80);

    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x60);
    tp28xx_byte_write(chip, 0x3a, 0x12);
    tp28xx_byte_write(chip, 0x3b, 0x26);
}

/////HDA QXGA25
static void TP2827C_AQXGAP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x13, 0x00);

    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x90);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x01);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0xa0);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0x6c);
    tp28xx_byte_write(chip, 0x32, 0xbe);
    tp28xx_byte_write(chip, 0x33, 0x80);

    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x60);
    tp28xx_byte_write(chip, 0x3a, 0x12);
    tp28xx_byte_write(chip, 0x3b, 0x26);
}


/////HDC QHD30
static void TP2827C_CQHDP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x40);
    tp28xx_byte_write(chip, 0x15, 0x13);
    tp28xx_byte_write(chip, 0x16, 0xfa);
    tp28xx_byte_write(chip, 0x18, 0x38);
    tp28xx_byte_write(chip, 0x1c, 0x0c);
    tp28xx_byte_write(chip, 0x1d, 0x80);

    tp28xx_byte_write(chip, 0x20, 0x50);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x38);

    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xda);

    tp28xx_byte_write(chip, 0x2d, 0x6c);
    tp28xx_byte_write(chip, 0x2e, 0x50);

    tp28xx_byte_write(chip, 0x30, 0x75);
    tp28xx_byte_write(chip, 0x31, 0x39);
    tp28xx_byte_write(chip, 0x32, 0xc0);
    tp28xx_byte_write(chip, 0x33, 0x31);

    tp28xx_byte_write(chip, 0x39, 0x48);
}

/////HDC QHD25
static void TP2827C_CQHDP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x40);
    tp28xx_byte_write(chip, 0x15, 0x13);
    tp28xx_byte_write(chip, 0x16, 0xfa);
    tp28xx_byte_write(chip, 0x18, 0x38);
    tp28xx_byte_write(chip, 0x1c, 0x0c);
    tp28xx_byte_write(chip, 0x1d, 0x80);

    tp28xx_byte_write(chip, 0x20, 0x50);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x38);

    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xda);

    tp28xx_byte_write(chip, 0x2d, 0x6c);
    tp28xx_byte_write(chip, 0x2e, 0x50);

    tp28xx_byte_write(chip, 0x30, 0x75);
    tp28xx_byte_write(chip, 0x31, 0x39);
    tp28xx_byte_write(chip, 0x32, 0xc0);
    tp28xx_byte_write(chip, 0x33, 0x3b);

    tp28xx_byte_write(chip, 0x39, 0x48);
}

///////HDA QHD15
static void TP2827C_AQHDP15_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x13, 0x00);

    tp28xx_byte_write(chip, 0x20, 0x38);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x01);

    tp28xx_byte_write(chip, 0x2d, 0x44);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x29);
    tp28xx_byte_write(chip, 0x31, 0x68);
    tp28xx_byte_write(chip, 0x32, 0x78);
    tp28xx_byte_write(chip, 0x33, 0x10);

    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x60);
    tp28xx_byte_write(chip, 0x3a, 0x12);
}

/////HDA QXGA18
static void TP2827C_AQXGAP18_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x13, 0x00);

    tp28xx_byte_write(chip, 0x15, 0x13);
    tp28xx_byte_write(chip, 0x16, 0x10);
    tp28xx_byte_write(chip, 0x18, 0x68);

    tp28xx_byte_write(chip, 0x20, 0x48);
    tp28xx_byte_write(chip, 0x21, 0x46);
    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x05);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2d, 0x52);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x29);
    tp28xx_byte_write(chip, 0x31, 0x65);
    tp28xx_byte_write(chip, 0x32, 0x2b);
    tp28xx_byte_write(chip, 0x33, 0xd0);

    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x60);
    tp28xx_byte_write(chip, 0x3a, 0x12);
}

/////TVI QHD30/QHD25
static void TP2827C_QHDP30_25_DataSet(unsigned char chip)
{
    unsigned char tmp;

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x1d);
    tp28xx_byte_write(chip, 0x18, 0x38);

    tp28xx_byte_write(chip, 0x20, 0x50);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x58);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0x58);
    tp28xx_byte_write(chip, 0x32, 0x9f);
    tp28xx_byte_write(chip, 0x33, 0x60);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x48);
    tp28xx_byte_write(chip, 0x3a, 0x12);
    tp28xx_byte_write(chip, 0x3b, 0x26);
}

/////TVI 5M20
static void TP2827C_5MP20_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x50);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x54);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0xa7);
    tp28xx_byte_write(chip, 0x32, 0x18);
    tp28xx_byte_write(chip, 0x33, 0x50);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x48);
    tp28xx_byte_write(chip, 0x3a, 0x12);
    tp28xx_byte_write(chip, 0x3b, 0x26);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}

/////HDA 5M20
static void TP2827C_A5MP20_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x20, 0x80);
    tp28xx_byte_write(chip, 0x21, 0x86);
    tp28xx_byte_write(chip, 0x22, 0x36);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x06);
    tp28xx_byte_write(chip, 0x27, 0xad);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2d, 0xA0);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x48);
    tp28xx_byte_write(chip, 0x31, 0x77);
    tp28xx_byte_write(chip, 0x32, 0x0e);
    tp28xx_byte_write(chip, 0x33, 0xa0);
    tp28xx_byte_write(chip, 0x39, 0x48);
}

/////TVI 8M15
static void TP2827C_8MP15_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x07, 0xc0);
    tp28xx_byte_write(chip, 0x0b, 0xc0);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x50);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x58);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0x59);
    tp28xx_byte_write(chip, 0x32, 0xbd);
    tp28xx_byte_write(chip, 0x33, 0x60);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x48);
    tp28xx_byte_write(chip, 0x3a, 0x12);
    tp28xx_byte_write(chip, 0x3b, 0x26);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
///////HDA 5M12.5
static void TP2827C_A5MP12_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x40;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x13, 0x00);

    tp28xx_byte_write(chip, 0x20, 0x38);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x01);

    tp28xx_byte_write(chip, 0x2d, 0x44);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x29);
    tp28xx_byte_write(chip, 0x31, 0x68);
    tp28xx_byte_write(chip, 0x32, 0x78);
    tp28xx_byte_write(chip, 0x33, 0x10);

    tp28xx_byte_write(chip, 0x18, 0x14);
    tp28xx_byte_write(chip, 0x1d, 0xb8);
    tp28xx_byte_write(chip, 0x36, 0xbc);

    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x60);
    tp28xx_byte_write(chip, 0x3a, 0x12);
}
///////////////////////////////////////////////////////////////////
static void TP2827C_A720P30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x02);
    tmp |= 0x04;
    tp28xx_byte_write(chip, 0x02, tmp);

    tp28xx_byte_write(chip, 0x0d, 0x70);

    tp28xx_byte_write(chip, 0x20, 0x40);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x01);

    tp28xx_byte_write(chip, 0x2c, 0x1a);
    tp28xx_byte_write(chip, 0x2d, 0x5a);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x9d);
    tp28xx_byte_write(chip, 0x31, 0xca);
    tp28xx_byte_write(chip, 0x32, 0x01);
    tp28xx_byte_write(chip, 0x33, 0xd0);
}
static void TP2827C_A720P25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x02);
    tmp |= 0x04;
    tp28xx_byte_write(chip, 0x02, tmp);

    tp28xx_byte_write(chip, 0x0d, 0x71);

    tp28xx_byte_write(chip, 0x20, 0x40);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x01);

    tp28xx_byte_write(chip, 0x2c, 0x1a);
    tp28xx_byte_write(chip, 0x2d, 0x5a);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0x9e);
    tp28xx_byte_write(chip, 0x31, 0x20);
    tp28xx_byte_write(chip, 0x32, 0x10);
    tp28xx_byte_write(chip, 0x33, 0x90);
}
static void TP2827C_A1080P30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x02);
    tmp |= 0x04;
    tp28xx_byte_write(chip, 0x02, tmp);

    tp28xx_byte_write(chip, 0x16, 0xf0);

    tp28xx_byte_write(chip, 0x0d, 0x72);

    tp28xx_byte_write(chip, 0x20, 0x38);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x01);

    tp28xx_byte_write(chip, 0x2c, 0x1a);
    tp28xx_byte_write(chip, 0x2d, 0x54);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0xa5);
    tp28xx_byte_write(chip, 0x31, 0x95);
    tp28xx_byte_write(chip, 0x32, 0xe0);
    tp28xx_byte_write(chip, 0x33, 0x60);
}
static void TP2827C_A1080P25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x02);
    tmp |= 0x04;
    tp28xx_byte_write(chip, 0x02, tmp);

    tp28xx_byte_write(chip, 0x16, 0xf0);

    tp28xx_byte_write(chip, 0x0d, 0x73);

    tp28xx_byte_write(chip, 0x20, 0x3c);
    tp28xx_byte_write(chip, 0x21, 0x46);

    tp28xx_byte_write(chip, 0x25, 0xfe);
    tp28xx_byte_write(chip, 0x26, 0x05);

    tp28xx_byte_write(chip, 0x2c, 0x1a);
    tp28xx_byte_write(chip, 0x2d, 0x54);
    tp28xx_byte_write(chip, 0x2e, 0x40);

    tp28xx_byte_write(chip, 0x30, 0xa5);
    tp28xx_byte_write(chip, 0x31, 0x86);
    tp28xx_byte_write(chip, 0x32, 0xfb);
    tp28xx_byte_write(chip, 0x33, 0x60);
}
