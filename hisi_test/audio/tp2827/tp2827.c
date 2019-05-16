static void TP2827_output(unsigned char chip)
{
    unsigned int tmp;

    tp28xx_byte_write(chip, 0xF1, 0x03);
   // tp28xx_byte_write(chip, 0x4D, 0x0f);
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
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);
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
        tp28xx_byte_write(chip, 0xF4, 0x00);
    }
    else if(DDR_2CH == output[chip])
    {
        tp28xx_byte_write(chip, 0xFA, 0x88);
        tp28xx_byte_write(chip, 0xFB, 0x00);
        //tp28xx_byte_write(chip, 0x45, 0x54); //PLL 297M
        tp28xx_byte_write(chip, 0xF4, 0xA0); //output clock 148.5M
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
        tp28xx_byte_write(chip, 0xFB, 0x00);
        //tp28xx_byte_write(chip, 0x45, 0x54); //PLL 297M
        tp28xx_byte_write(chip, 0xF4, 0xA0); //output clock 148.5M
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
        tp28xx_byte_write(chip, 0xF4, 0xA0); //output clock 148.5M
        tp28xx_byte_write(chip, 0xF6, 0x40);
        tp28xx_byte_write(chip, 0xF7, 0x51);
        tp28xx_byte_write(chip, 0xF8, 0x62);
        tp28xx_byte_write(chip, 0xF9, 0x73);
        tp28xx_byte_write(chip, 0x50, 0x00); //
        tp28xx_byte_write(chip, 0x51, 0x00); //
        tp28xx_byte_write(chip, 0x52, 0x00); //
        tp28xx_byte_write(chip, 0x53, 0x00); //
        tp28xx_byte_write(chip, 0xF3, 0x77);
        tp28xx_byte_write(chip, 0xF2, 0x77);
    }

    printk("\ntp28xx_byte_read(chip, 0x4D) 0x%x\n",tp28xx_byte_read(chip, 0x4D));
    tp28xx_byte_write(chip, 0x4D, 0x0f);
    tp28xx_byte_write(chip, 0x4E, 0x0f);
}
/////HDA QHD30
static void TP2827_AQHDP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x16);
    tp28xx_byte_write(chip, 0x18, 0x32);

            tp28xx_byte_write(chip, 0x20, 0x90);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0xad);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0xA0);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x6a);
            tp28xx_byte_write(chip, 0x32, 0xbe);
            tp28xx_byte_write(chip, 0x33, 0x80);
            tp28xx_byte_write(chip, 0x35, 0x15);
            tp28xx_byte_write(chip, 0x39, 0x4c);
}
static void TP2827_AQHDH30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
                        tp28xx_byte_write(chip, 0x20, 0x48);
                        tp28xx_byte_write(chip, 0x21, 0x86);
                        tp28xx_byte_write(chip, 0x22, 0x36);

                        tp28xx_byte_write(chip, 0x25, 0xff);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x2d);

                        tp28xx_byte_write(chip, 0x2b, 0x60);
                        tp28xx_byte_write(chip, 0x2d, 0x50);
                        tp28xx_byte_write(chip, 0x2e, 0x50);

                        tp28xx_byte_write(chip, 0x30, 0x48);
                        tp28xx_byte_write(chip, 0x31, 0x6a);
                        tp28xx_byte_write(chip, 0x32, 0xbe);
                        tp28xx_byte_write(chip, 0x33, 0x80);
                        tp28xx_byte_write(chip, 0x35, 0x15);
                        //tp28xx_byte_write(chip, 0x39, 0x1c);
}
/////HDA QHD25
static void TP2827_AQHDP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x16);
    tp28xx_byte_write(chip, 0x18, 0x32);

            tp28xx_byte_write(chip, 0x20, 0x90);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0xad);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0xA0);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x6f);
            tp28xx_byte_write(chip, 0x32, 0xb5);
            tp28xx_byte_write(chip, 0x33, 0x80);
            tp28xx_byte_write(chip, 0x35, 0x15);
            tp28xx_byte_write(chip, 0x39, 0x4c);
}
static void TP2827_AQHDH25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
                        tp28xx_byte_write(chip, 0x20, 0x48);
                        tp28xx_byte_write(chip, 0x21, 0x86);
                        tp28xx_byte_write(chip, 0x22, 0x36);

                        tp28xx_byte_write(chip, 0x25, 0xff);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x2d);

                        tp28xx_byte_write(chip, 0x2b, 0x60);
                        tp28xx_byte_write(chip, 0x2d, 0x50);
                        tp28xx_byte_write(chip, 0x2e, 0x40);

                        tp28xx_byte_write(chip, 0x30, 0x48);
                        tp28xx_byte_write(chip, 0x31, 0x6f);
                        tp28xx_byte_write(chip, 0x32, 0xb5);
                        tp28xx_byte_write(chip, 0x33, 0x80);
                        tp28xx_byte_write(chip, 0x35, 0x15);
                        //tp28xx_byte_write(chip, 0x39, 0x1c);
}
/////HDA QXGA30
static void TP2827_AQXGAP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);
            tp28xx_byte_write(chip, 0x20, 0x90);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0xad);
            tp28xx_byte_write(chip, 0x28, 0x00);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0xA0);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x68);
            tp28xx_byte_write(chip, 0x32, 0xbe);
            tp28xx_byte_write(chip, 0x33, 0x80);
            tp28xx_byte_write(chip, 0x39, 0x4c);
}
static void TP2827_AQXGAH30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);
                        tp28xx_byte_write(chip, 0x20, 0x48);
                        tp28xx_byte_write(chip, 0x21, 0x86);
                        tp28xx_byte_write(chip, 0x22, 0x36);

                        tp28xx_byte_write(chip, 0x25, 0xff);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x2d);

                        tp28xx_byte_write(chip, 0x2b, 0x60);
                        tp28xx_byte_write(chip, 0x2d, 0x50);
                        tp28xx_byte_write(chip, 0x2e, 0x50);

                        tp28xx_byte_write(chip, 0x30, 0x48);
                        tp28xx_byte_write(chip, 0x31, 0x68);
                        tp28xx_byte_write(chip, 0x32, 0xbe);
                        tp28xx_byte_write(chip, 0x33, 0x80);
}
/////HDA QXGA25
static void TP2827_AQXGAP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
    tp28xx_byte_write(chip, 0x13, 0x00);
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);
            tp28xx_byte_write(chip, 0x20, 0x90);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0xad);
            tp28xx_byte_write(chip, 0x28, 0x00);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0xA0);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x6c);
            tp28xx_byte_write(chip, 0x32, 0xbe);
            tp28xx_byte_write(chip, 0x33, 0x80);
            tp28xx_byte_write(chip, 0x39, 0x4c);
}
static void TP2827_AQXGAH25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

                        tp28xx_byte_write(chip, 0x20, 0x48);
                        tp28xx_byte_write(chip, 0x21, 0x86);
                        tp28xx_byte_write(chip, 0x22, 0x36);

                        tp28xx_byte_write(chip, 0x25, 0xff);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x2d);

                        tp28xx_byte_write(chip, 0x2b, 0x60);
                        tp28xx_byte_write(chip, 0x2d, 0x50);
                        tp28xx_byte_write(chip, 0x2e, 0x40);

                        tp28xx_byte_write(chip, 0x30, 0x48);
                        tp28xx_byte_write(chip, 0x31, 0x6c);
                        tp28xx_byte_write(chip, 0x32, 0xb5);
                        tp28xx_byte_write(chip, 0x33, 0x80);
}

/////HDC QHD30
static void TP2827_CQHDP30_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x40);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x16);
    tp28xx_byte_write(chip, 0x18, 0x32);

                        tp28xx_byte_write(chip, 0x20, 0x68);
                        tp28xx_byte_write(chip, 0x26, 0x05);
                        tp28xx_byte_write(chip, 0x27, 0xda);

                        tp28xx_byte_write(chip, 0x2d, 0x70);
                        tp28xx_byte_write(chip, 0x2e, 0x50);

                        tp28xx_byte_write(chip, 0x30, 0x75);
                        tp28xx_byte_write(chip, 0x31, 0x39);
                        tp28xx_byte_write(chip, 0x32, 0xc0);
                        tp28xx_byte_write(chip, 0x33, 0x30);
                        //tp28xx_byte_write(chip, 0x35, 0x95);
                        //tp28xx_byte_write(chip, 0x39, 0x0c);
}
static void TP2827_CQHDH30_DataSet(unsigned char chip)
{
                        tp28xx_byte_write(chip, 0x20, 0x34);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x5a);

                        tp28xx_byte_write(chip, 0x2d, 0x38);
                        tp28xx_byte_write(chip, 0x2e, 0x40);

                        tp28xx_byte_write(chip, 0x30, 0x75);
                        tp28xx_byte_write(chip, 0x31, 0x39);
                        tp28xx_byte_write(chip, 0x32, 0xc0);
                        tp28xx_byte_write(chip, 0x33, 0x30);
                        //tp28xx_byte_write(chip, 0x35, 0x95);
                        //tp28xx_byte_write(chip, 0x39, 0x0c);
                        tp28xx_byte_write(chip, 0x13, 0x40);
}
/////HDC QHD25
static void TP2827_CQHDP25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

    tp28xx_byte_write(chip, 0x13, 0x40);
    tp28xx_byte_write(chip, 0x15, 0x23);
    tp28xx_byte_write(chip, 0x16, 0x80);
    tp28xx_byte_write(chip, 0x18, 0x32);
                        tp28xx_byte_write(chip, 0x20, 0x68);
                        tp28xx_byte_write(chip, 0x26, 0x05);
                        tp28xx_byte_write(chip, 0x27, 0xda);

                        tp28xx_byte_write(chip, 0x2d, 0x84);
                        tp28xx_byte_write(chip, 0x2e, 0x50);

                        tp28xx_byte_write(chip, 0x30, 0x75);
                        tp28xx_byte_write(chip, 0x31, 0x39);
                        tp28xx_byte_write(chip, 0x32, 0xc0);
                        tp28xx_byte_write(chip, 0x33, 0x30);
                        //tp28xx_byte_write(chip, 0x35, 0x95);
                        //tp28xx_byte_write(chip, 0x39, 0x0c);
}
static void TP2827_CQHDH25_DataSet(unsigned char chip)
{
                        tp28xx_byte_write(chip, 0x20, 0x34);
                        tp28xx_byte_write(chip, 0x26, 0x01);
                        tp28xx_byte_write(chip, 0x27, 0x5a);

                        tp28xx_byte_write(chip, 0x2d, 0x42);
                        tp28xx_byte_write(chip, 0x2e, 0x40);

                        tp28xx_byte_write(chip, 0x30, 0x75);
                        tp28xx_byte_write(chip, 0x31, 0x39);
                        tp28xx_byte_write(chip, 0x32, 0xc0);
                        tp28xx_byte_write(chip, 0x33, 0x30);
                        //tp28xx_byte_write(chip, 0x35, 0x95);
                        //tp28xx_byte_write(chip, 0x39, 0x0c);
                        tp28xx_byte_write(chip, 0x13, 0x40);

}
///////HDA QHD15
static void TP2827_AQHDP15_DataSet(unsigned char chip)
{
    unsigned char tmp;
                        tmp = tp28xx_byte_read(chip, 0x14);
                        tmp |= 0x60;
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

                        //tp28xx_byte_write(chip, 0x3B, 0x05);

}
/////HDA QXGA18
static void TP2827_AQXGAP18_DataSet(unsigned char chip)
{
    unsigned char tmp;
                        tmp = tp28xx_byte_read(chip, 0x14);
                        tmp |= 0x60;
                        tp28xx_byte_write(chip, 0x14, tmp);
                        tp28xx_byte_write(chip, 0x13, 0x00);

                        tp28xx_byte_write(chip, 0x15, 0x13);
                        tp28xx_byte_write(chip, 0x16, 0x10);
                        tp28xx_byte_write(chip, 0x18, 0x68);

                        tp28xx_byte_write(chip, 0x20, 0x48);
                        tp28xx_byte_write(chip, 0x21, 0x46);
                        tp28xx_byte_write(chip, 0x25, 0xfe);
                        tp28xx_byte_write(chip, 0x26, 0x01);

                        tp28xx_byte_write(chip, 0x2b, 0x60);
                        tp28xx_byte_write(chip, 0x2d, 0x52);
                        tp28xx_byte_write(chip, 0x2e, 0x40);

                        tp28xx_byte_write(chip, 0x30, 0x29);
                        tp28xx_byte_write(chip, 0x31, 0x65);
                        tp28xx_byte_write(chip, 0x32, 0x2b);
                        tp28xx_byte_write(chip, 0x33, 0xd0);

}
/////TVI QHD30/QHD25
static void TP2827_QHDP30_25_DataSet(unsigned char chip)
{
    unsigned char tmp;

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);

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

    //tp28xx_byte_write(chip, 0x30, 0x73);
    //tp28xx_byte_write(chip, 0x31, 0xf5);
    //tp28xx_byte_write(chip, 0x32, 0x3e);
    //tp28xx_byte_write(chip, 0x33, 0x30);
    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0x58);
    tp28xx_byte_write(chip, 0x32, 0x9f);
    tp28xx_byte_write(chip, 0x33, 0x60);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x4c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3b, 0x27);
}
static void TP2827_QHDH30_25_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x28);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x2c);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0x58);
    tp28xx_byte_write(chip, 0x32, 0x9f);
    tp28xx_byte_write(chip, 0x33, 0x60);
    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x4c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3b, 0x27);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
/////TVI 5M20
static void TP2827_5MP20_DataSet(unsigned char chip)
{
    unsigned char tmp;
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

    //tp28xx_byte_write(chip, 0x30, 0x73);
    //tp28xx_byte_write(chip, 0x31, 0xf7);
    //tp28xx_byte_write(chip, 0x32, 0xce);
    //tp28xx_byte_write(chip, 0x33, 0xe0);
    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0xa7);
    tp28xx_byte_write(chip, 0x32, 0x18);
    tp28xx_byte_write(chip, 0x33, 0x50);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x4c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3b, 0x27);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
static void TP2827_5MH20_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tp28xx_byte_write(chip, 0x0c, 0x03);
    tp28xx_byte_write(chip, 0x0d, 0x50);

    tp28xx_byte_write(chip, 0x20, 0x28);
    tp28xx_byte_write(chip, 0x21, 0x84);
    tp28xx_byte_write(chip, 0x22, 0x36);
    tp28xx_byte_write(chip, 0x23, 0x3c);

    tp28xx_byte_write(chip, 0x25, 0xff);
    tp28xx_byte_write(chip, 0x26, 0x05);
    tp28xx_byte_write(chip, 0x27, 0xad);
    tp28xx_byte_write(chip, 0x28, 0x00);

    tp28xx_byte_write(chip, 0x2b, 0x60);
    tp28xx_byte_write(chip, 0x2c, 0x0a);
    tp28xx_byte_write(chip, 0x2d, 0x2c);
    tp28xx_byte_write(chip, 0x2e, 0x70);

    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0xa7);
    tp28xx_byte_write(chip, 0x32, 0x18);
    tp28xx_byte_write(chip, 0x33, 0x50);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x4c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3b, 0x27);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
/////HDA 5M20
static void TP2827_A5MP20_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
            tp28xx_byte_write(chip, 0x20, 0x90);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0xad);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0xA0);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x77);
            tp28xx_byte_write(chip, 0x32, 0x0a);
            tp28xx_byte_write(chip, 0x33, 0xa0);
            tp28xx_byte_write(chip, 0x39, 0x4c);
}
static void TP2827_A5MH20_DataSet(unsigned char chip)
{
    unsigned char tmp;
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp |= 0x60;
    tp28xx_byte_write(chip, 0x14, tmp);
            tp28xx_byte_write(chip, 0x20, 0x48);
            tp28xx_byte_write(chip, 0x21, 0x86);
            tp28xx_byte_write(chip, 0x22, 0x36);

            tp28xx_byte_write(chip, 0x25, 0xff);
            tp28xx_byte_write(chip, 0x26, 0x01);
            tp28xx_byte_write(chip, 0x27, 0x2d);

            tp28xx_byte_write(chip, 0x2b, 0x60);
            tp28xx_byte_write(chip, 0x2d, 0x50);
            tp28xx_byte_write(chip, 0x2e, 0x40);

            tp28xx_byte_write(chip, 0x30, 0x48);
            tp28xx_byte_write(chip, 0x31, 0x77);
            tp28xx_byte_write(chip, 0x32, 0x0a);
            tp28xx_byte_write(chip, 0x33, 0xa0);
            //tp28xx_byte_write(chip, 0x39, 0x4c);
}

/////TVI 8M15
static void TP2827_8MP15_DataSet(unsigned char chip)
{
    unsigned char tmp;
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

    //tp28xx_byte_write(chip, 0x30, 0x73);
    //tp28xx_byte_write(chip, 0x31, 0xf7);
    //tp28xx_byte_write(chip, 0x32, 0xce);
    //tp28xx_byte_write(chip, 0x33, 0xe0);
    tp28xx_byte_write(chip, 0x30, 0x74);
    tp28xx_byte_write(chip, 0x31, 0x59);
    tp28xx_byte_write(chip, 0x32, 0xbd);
    tp28xx_byte_write(chip, 0x33, 0x60);

    //tp28xx_byte_write(chip, 0x35, 0x05);
    tp28xx_byte_write(chip, 0x38, 0x40);
    tp28xx_byte_write(chip, 0x39, 0x4c);
    tp28xx_byte_write(chip, 0x3a, 0x32);
    tp28xx_byte_write(chip, 0x3b, 0x27);

    tp28xx_byte_write(chip, 0x13, 0x00);
    tmp = tp28xx_byte_read(chip, 0x14);
    tmp &= 0x9f;
    tp28xx_byte_write(chip, 0x14, tmp);
}
///////HDA 5M12.5
static void TP2827_A5MP12_DataSet(unsigned char chip)
{
    unsigned char tmp;
                        tmp = tp28xx_byte_read(chip, 0x14);
                        tmp |= 0x60;
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

                        //tp28xx_byte_write(chip, 0x3B, 0x05);

                        tp28xx_byte_write(chip, 0x18, 0x14);
                        tp28xx_byte_write(chip, 0x1d, 0xb8);
                        tp28xx_byte_write(chip, 0x36, 0xbc);

}
