// credits to davidjo for making speakers work on mbp14,3
// below is the setup for mb9,1

//this line is only needed for kernel 5.0 and below
#define AC_VERB_GET_STRIPE_CONTROL		0x0f24

static inline unsigned int snd_hda_codec_read_check(struct hda_codec *codec, hda_nid_t nid, int flags, unsigned int verb, unsigned int parm, unsigned int check_val, int srcidx)
{
        unsigned int retval;
        retval = snd_hda_codec_read(codec, nid, flags, verb, parm);

        if (retval == -1)
                return retval;

        if (retval != check_val)
                codec_dbg(codec, "command nid BAD read check return value at %d: 0x%08x 0x%08x\n",srcidx,retval,check_val);

        return retval;
}
static inline void cs_4208_vendor_coef_set(struct hda_codec *codec, unsigned int idx,
                                      unsigned int coef)
{
        struct cs_spec *spec = codec->spec;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_PROC_COEF, coef);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        // appears to return 0
}
static inline unsigned int cs_4208_vendor_coef_get(struct hda_codec *codec, unsigned int idx)
{
        struct cs_spec *spec = codec->spec;
        unsigned int retval;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        retval = snd_hda_codec_read(codec, spec->vendor_nid, 0,
                                  AC_VERB_GET_PROC_COEF, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        return retval;
}
static inline unsigned int cs_4208_vendor_coef_set_mask(struct hda_codec *codec, unsigned int idx,
                                      unsigned int coef, unsigned int mask)
{
        struct cs_spec *spec = codec->spec;
        unsigned int retval;
        //unsigned int mask_coef;
        snd_hda_codec_read(codec, spec->vendor_nid, 0,
                            AC_VERB_GET_COEF_INDEX, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        retval = snd_hda_codec_read(codec, spec->vendor_nid, 0,
                                  AC_VERB_GET_PROC_COEF, 0);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, idx);
        //mask_coef = (retval & ~mask) | coef;
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_PROC_COEF, coef);
        snd_hda_codec_write(codec, spec->vendor_nid, 0,
                            AC_VERB_SET_COEF_INDEX, 0);
        // appears to return 0
        // lets return the read value for checking
        return retval;
}
void snd_hda_coef_item(struct hda_codec *codec, u16 write_flag, hda_nid_t nid, u32 idx, u32 param, u32 retdata, int srcidx)
{
        if (write_flag == 2)
        {
                unsigned int retval = cs_4208_vendor_coef_set_mask(codec, idx, param, 0);
                if (retval != retdata)
                {
                        if (srcidx > 0)
                                codec_dbg(codec, "command nid BAD mask return value at %d: 0x%08x 0x%08x\n",srcidx,retval,retdata);
                        else
                                codec_dbg(codec, "command nid BAD mask return value: 0x%08x 0x%08x\n",retval,retdata);
                }
        }
        else if (write_flag == 1)
                cs_4208_vendor_coef_set(codec, idx, param);
        else
        {
                unsigned int retval = cs_4208_vendor_coef_get(codec, idx);
                if (retval != retdata)
                {
                        if (srcidx > 0)
                                codec_dbg(codec, "command nid BAD      return value at %d: 0x%08x 0x%08x\n",srcidx,retval,retdata);
                        else
                                codec_dbg(codec, "command nid BAD      return value: 0x%08x 0x%08x\n",retval,retdata);
                }
        }
}

/* reserved for future use
static int headphones_a1534 (struct hda_codec *codec) {
	int retval;
	codec_dbg(codec, "headphones_a1534 start");

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x80000000, 2); // 0x010f0900
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 3); // 0x011f0900
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 4); // 0x017f0900
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 5); // 0x018f0900
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 6); // 0x021f0900
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 7); // 0x022f0900

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 13); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 16); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 18); // 0x010f0500

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (27)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 27
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 28 ); //   coef read 28

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 (31)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef write 31
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 32 ); //   coef write 32

       // read  node 0x24 arg 1 0x0000 arg 2 0x001f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (35)
 //       { 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000 }, //   coef read 35
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000, 36 ); //   coef read 36
//        vendor_verb
        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x000000ae);


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000000c4 (41)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4 }, //   coef read 41
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4, 42 ); //   coef read 42

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x20c4 arg 4 0x0000 (45)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x20c4, 0x00000000 }, //   coef write 45
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x20c4, 0x00000000, 46 ); //   coef write 46

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000020); // 0x00171520
//       snd_hda:     gpio data 1 0x20


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000030); // 0x00171530
//       snd_hda:     gpio data 1 0x30


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_CONNECT_SEL, 0x00000001); // 0x00770101


        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 84); // 0x018f0700

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01870500

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 87); // 0x018f0500

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (88)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 88
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 89 ); //   coef read 89

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 (92)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef write 92
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 93 ); //   coef write 93

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 97); // 0x018f0700
        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000004); // 0x01870704
//       snd_hda:     24 []



       // read  node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 data 0x00004383 (99)
 //       { 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00004383 }, //   coef read 99
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00004383, 100 ); //   coef read 100

       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0xc383 arg 4 0x0000 (103)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0xc383, 0x00000000 }, //   coef write 103
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0xc383, 0x00000000, 104 ); //   coef write 104


        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);



       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x8383 arg 4 0x0000 (111)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000 }, //   coef write 111
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000, 112 ); //   coef write 112


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000010); // 0x00171510
//       snd_hda:     gpio data 1 0x10


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000020c4 (130)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000020c4 }, 
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000020c4, 131 ); 
       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (134)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, 
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 135 ); 
       snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_CONNECT_SEL, 0x00000001); // 0x00770101


        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000004, 141); // 0x018f0700
        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01870720
//       snd_hda:     24 ['AC_PINCTL_IN_EN']


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (143)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef read 143
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 144 ); //   coef read 144

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0010 arg 4 0x0000 (147)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0010, 0x00000000 }, //   coef write 147
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0010, 0x00000000, 148 ); //   coef write 148

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000020, 152); // 0x018f0700

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01870503

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 154); // 0x018f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 157); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 160); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 162); // 0x010f0500



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (164)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 164
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 165 ); //   coef read 165

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (168)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 168
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 169 ); //   coef write 169

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 174); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 177); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00270610
//       snd_hda:     conv stream channel map 2 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00270503

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 180); // 0x002f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01070100

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000c2, 183); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c2 mute 1 gain 0x42 66
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a0c1); // 0x0103a0c1
//       snd_hda:     amp gain/mute 16 0xa0c1 mute 1 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000c2, 185); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c2 mute 1 gain 0x42 66
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000090c1); // 0x010390c1
//       snd_hda:     amp gain/mute 16 0x90c1 mute 1 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000c1, 187); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c1 mute 1 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000c1, 189); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x00c1 mute 1 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 191); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 193); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000ff, 195); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000ff, 197); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 199); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 201); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 203); // 0x010f0700
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01070740
//       snd_hda:     16 ['AC_PINCTL_OUT_EN']



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (206)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 206
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 207 ); //   coef read 207

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (210)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 210
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 211 ); //   coef write 211

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 218); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000090ff); // 0x002390ff
//       snd_hda:     amp gain/mute 2 0x90ff mute 1 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 221); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a0ff); // 0x0023a0ff
//       snd_hda:     amp gain/mute 2 0xa0ff mute 1 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x000000ff, 224); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x000000ff, 227); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x00ff mute 1 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 230); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 232); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 235); // 0x006f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 236); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 237); // 0x002f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 240); // 0x010f0500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 243); // 0x010f0500
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 245); // 0x010f0500

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 254); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 270); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 274); // 0x000f0000

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 280); // 0x010f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 283); // 0x002f0500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 286); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 287); // 0x010f0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (288)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 288
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 289 ); //   coef read 289

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (292)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 292
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 293 ); //   coef write 293

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00224031
//       snd_hda:     stream format 2 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 298); // 0x002f0500

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00270610
//       snd_hda:     conv stream channel map 2 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01070100

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 301); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 303); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 305); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 307); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000041, 309); // 0x010ba000
//       snd_hda:     amp gain/mute 16 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a041); // 0x0103a041
//       snd_hda:     amp gain/mute 16 0xa041 mute 0 gain 0x41 65 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000041, 311); // 0x010b8000
//       snd_hda:     amp gain/mute 16 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 16 0x0041 mute 0 gain 0x41 65
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009041); // 0x01039041
//       snd_hda:     amp gain/mute 16 0x9041 mute 0 gain 0x41 65 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x0000007f, 313); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x0000007f, 315); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x007f mute 0 gain 0x7f 127
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 317); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a080); // 0x0023a080
//       snd_hda:     amp gain/mute 2 0xa080 mute 1 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 319); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009080); // 0x00239080
//       snd_hda:     amp gain/mute 2 0x9080 mute 1 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 321); // 0x010f0700
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01070740
//       snd_hda:     16 ['AC_PINCTL_OUT_EN']


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000080, 324); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000080, 326); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0080 mute 1 gain 0x0 0
        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 329); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a000); // 0x0023a000
//       snd_hda:     amp gain/mute 2 0xa000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 332); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00009000); // 0x00239000
//       snd_hda:     amp gain/mute 2 0x9000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 1 input 0  right output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x0000a000, 0x00000000, 335); // 0x002ba000
//       snd_hda:     amp gain/mute 2 0xa000 index 0x00 left/right 1 left output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000a07f); // 0x0023a07f
//       snd_hda:     amp gain/mute 2 0xa07f mute 0 gain 0x7f 127 index 0x00 left 1 right 0 output 1 input 0 left  output 


        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00008000, 0x00000000, 338); // 0x002b8000
//       snd_hda:     amp gain/mute 2 0x8000 index 0x00 left/right 0 right output/input 1 output
//       snd_hda:     amp gain/mute 2 0x0000 mute 0 gain 0x0 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x0000907f); // 0x0023907f
//       snd_hda:     amp gain/mute 2 0x907f mute 0 gain 0x7f 127 index 0x00 left 0 right 1 output 1 input 0  right output 

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 340); // 0x006f0500
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 341); // 0x010f0500
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 342); // 0x002f0500

	codec_dbg(codec, "headphones_a1534 end");
	return 0;
}
*/

static int setup_a1534 (struct hda_codec *codec) {
	int retval;
	codec_dbg(codec, "setup_a1534 start");

	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 2); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 3); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 4); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00010001, 5); // 0x000f0004
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00020023, 6); // 0x001f0004
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000101, 7); // 0x001f0005
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_SUBSYSTEM_ID, 0x00000000, 0x106b6500, 8); // 0x001f2000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 9); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 10); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000f, 0xe0000019, 11); // 0x001f000f
	//snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_DBL_CODEC_RESET, 0x00000000); // 0x001fff00
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 13); // 0x000f0000
	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 14); // 0x000f0002
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000101, 15); // 0x001f0005
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000f, 0xe0000019, 16); // 0x001f000f
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 17); // 0x001f000a
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 18); // 0x001f000b
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 19); // 0x001f0012
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 20); // 0x001f000d
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_SUBSYSTEM_ID, 0x00000000, 0x106b6500, 22); // 0x001f2000
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000008, 0x00010000, 23); // 0x001f0008
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DIRECTION, 0x00000000, 0x00000000, 24); // 0x001f1700
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000011, 0xc0000206, 25); // 0x001f0011
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00020023, 26); // 0x001f0004
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 27); // 0x002f0005
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 28); // 0x002f0009
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 29); // 0x002f000f
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 30); // 0x002f000a
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 31); // 0x002f000b
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 32); // 0x002f0012
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 33); // 0x002f000d
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 34); // 0x002f0009
	snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 36); // 0x002f000a
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 37); // 0x002f000b
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 38); // 0x002f0012
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 39); // 0x002f000f
	retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 40); // 0x002f2400
	snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00270503
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 43); // 0x003f0005
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 44); // 0x003f0009
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 45); // 0x003f000f
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 46); // 0x003f000a
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 47); // 0x003f000b
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 48); // 0x003f0012
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 49); // 0x003f000d
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 50); // 0x003f0009
	snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00370500
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 52); // 0x003f000a
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 53); // 0x003f000b
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 54); // 0x003f0012
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 55); // 0x003f000f
	retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 56); // 0x003f2400
	snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00370503
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 59); // 0x004f0005
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 60); // 0x004f0009
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 61); // 0x004f000f
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 62); // 0x004f000a
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 63); // 0x004f000b
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 64); // 0x004f0012
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 65); // 0x004f000d
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 66); // 0x004f0009
	snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00470500
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 68); // 0x004f000a
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 69); // 0x004f000b
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 70); // 0x004f0012
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 71); // 0x004f000f
	retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 72); // 0x004f2400
	snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00470503
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 75); // 0x005f0005
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 76); // 0x005f0009
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 77); // 0x005f000f
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 78); // 0x005f000a
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 79); // 0x005f000b
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 80); // 0x005f0012
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 81); // 0x005f000d
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 82); // 0x005f0009
	snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00570500
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 84); // 0x005f000a
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 85); // 0x005f000b
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 86); // 0x005f0012
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 87); // 0x005f000f
	retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 88); // 0x005f2400
	snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00570503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 91); // 0x006f0005
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 92); // 0x006f0009
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 93); // 0x006f000f
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 94); // 0x006f000a
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 95); // 0x006f000b
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 96); // 0x006f0012
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 97); // 0x006f000d
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 98); // 0x006f0009
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 100); // 0x006f000a
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 101); // 0x006f000b
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 102); // 0x006f000d
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 103); // 0x006f000e
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00191715, 104); // 0x006f0200
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 105); // 0x006f000f
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 108); // 0x007f0005
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 109); // 0x007f0009
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 110); // 0x007f000f
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 111); // 0x007f000a
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 112); // 0x007f000b
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 113); // 0x007f0012
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 114); // 0x007f000d
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 115); // 0x007f0009
	snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00770500
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 117); // 0x007f000a
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 118); // 0x007f000b
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 119); // 0x007f000d
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 120); // 0x007f000e
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x001a1816, 121); // 0x007f0200
	retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 122); // 0x007f000f
	snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00770503
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 125); // 0x008f0005
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 126); // 0x008f0009
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 127); // 0x008f000f
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 128); // 0x008f000a
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 129); // 0x008f000b
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 130); // 0x008f0012
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 131); // 0x008f000d
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 132); // 0x008f0009
	snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00870500
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 134); // 0x008f000a
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 135); // 0x008f000b
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 136); // 0x008f000d
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 137); // 0x008f000e
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001b, 138); // 0x008f0200
	retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 139); // 0x008f000f
	snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00870503
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 142); // 0x009f0005
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 143); // 0x009f0009
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 144); // 0x009f000f
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 145); // 0x009f000a
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 146); // 0x009f000b
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 147); // 0x009f0012
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 148); // 0x009f000d
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 149); // 0x009f0009
	snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00970500
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 151); // 0x009f000a
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 152); // 0x009f000b
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 153); // 0x009f000d
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 154); // 0x009f000e
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001c, 155); // 0x009f0200
	retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 156); // 0x009f000f
	snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00970503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 159); // 0x00af0005
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 160); // 0x00af0009
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 161); // 0x00af000f
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 162); // 0x00af000a
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 163); // 0x00af000b
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 164); // 0x00af0012
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 165); // 0x00af000d
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 166); // 0x00af0009
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 168); // 0x00af000a
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 169); // 0x00af000b
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 170); // 0x00af000f
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 171); // 0x00af2400
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 174); // 0x00bf0005
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 175); // 0x00bf0009
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 176); // 0x00bf000f
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 177); // 0x00bf000a
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 178); // 0x00bf000b
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 179); // 0x00bf0012
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 180); // 0x00bf000d
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 181); // 0x00bf0009
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 183); // 0x00bf000a
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 184); // 0x00bf000b
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 185); // 0x00bf000f
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 186); // 0x00bf2400
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 189); // 0x00cf0005
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 190); // 0x00cf0009
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 191); // 0x00cf000f
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 192); // 0x00cf000a
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 193); // 0x00cf000b
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 194); // 0x00cf0012
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 195); // 0x00cf000d
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 196); // 0x00cf0009
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 198); // 0x00cf000a
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 199); // 0x00cf000b
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 200); // 0x00cf000e
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001f, 201); // 0x00cf0200
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 202); // 0x00cf000f
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 205); // 0x00df0005
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 206); // 0x00df0009
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 207); // 0x00df000f
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 208); // 0x00df000a
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 209); // 0x00df000b
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 210); // 0x00df0012
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 211); // 0x00df000d
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 212); // 0x00df0009
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 214); // 0x00df000a
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 215); // 0x00df000b
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 216); // 0x00df000e
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000020, 217); // 0x00df0200
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 218); // 0x00df000f
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 221); // 0x00ef0005
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 222); // 0x00ef0009
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 223); // 0x00ef000f
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 224); // 0x00ef000a
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 225); // 0x00ef000b
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 226); // 0x00ef0012
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 227); // 0x00ef000d
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 228); // 0x00ef0009
	snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00e70500
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 230); // 0x00ef000a
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 231); // 0x00ef000b
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 232); // 0x00ef000f
	retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 233); // 0x00ef2400
	snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00e70503
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 236); // 0x00ff0005
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 237); // 0x00ff0009
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 238); // 0x00ff000f
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 239); // 0x00ff000a
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 240); // 0x00ff000b
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 241); // 0x00ff0012
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 242); // 0x00ff000d
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 243); // 0x00ff0009
	snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00f70500
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 245); // 0x00ff000a
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 246); // 0x00ff000b
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 247); // 0x00ff000e
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000022, 248); // 0x00ff0200
	retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 249); // 0x00ff000f
	snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00f70503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 252); // 0x010f0005
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 253); // 0x010f0009
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 254); // 0x010f000f
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 255); // 0x010f0012
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 256); // 0x010f000d
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 257); // 0x010f0009
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x002b4020, 259); // 0x010f1c00
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x0000001c, 260); // 0x010f000c
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 261); // 0x010f0012
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 262); // 0x010f000e
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 263); // 0x010f0200
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 264); // 0x010f000f
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 265); // 0x010f0700
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01070700
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 269); // 0x011f0005
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 270); // 0x011f0009
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 271); // 0x011f000f
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 272); // 0x011f0012
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 273); // 0x011f000d
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 274); // 0x011f0009
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01170500
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 276); // 0x011f1c00
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000054, 277); // 0x011f000c
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 278); // 0x011f000e
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 279); // 0x011f0200
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 280); // 0x011f000f
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 281); // 0x011f0700
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01170700
	snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01170503
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 285); // 0x012f0005
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 286); // 0x012f0009
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 287); // 0x012f000f
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 288); // 0x012f0012
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 289); // 0x012f000d
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 290); // 0x012f0009
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01270500
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 292); // 0x012f1c00
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 293); // 0x012f000c
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 294); // 0x012f000e
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000003, 295); // 0x012f0200
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 296); // 0x012f000f
	retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 297); // 0x012f0700
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01270700
	snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01270503
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 301); // 0x013f0005
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 302); // 0x013f0009
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 303); // 0x013f000f
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 304); // 0x013f0012
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 305); // 0x013f000d
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 306); // 0x013f0009
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01370500
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 308); // 0x013f1c00
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 309); // 0x013f000c
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 310); // 0x013f000e
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000004, 311); // 0x013f0200
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 312); // 0x013f000f
	retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 313); // 0x013f0700
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01370700
	snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01370503
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 317); // 0x014f0005
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 318); // 0x014f0009
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 319); // 0x014f000f
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 320); // 0x014f0012
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 321); // 0x014f000d
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 322); // 0x014f0009
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01470500
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 324); // 0x014f1c00
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 325); // 0x014f000c
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 326); // 0x014f000e
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000005, 327); // 0x014f0200
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 328); // 0x014f000f
	retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 329); // 0x014f0700
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01470700
	snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01470503
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 333); // 0x015f0005
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 334); // 0x015f0009
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 335); // 0x015f000f
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 336); // 0x015f0012
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 337); // 0x015f000d
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 338); // 0x015f0009
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01570500
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 340); // 0x015f1c00
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 341); // 0x015f000c
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 342); // 0x015f000d
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 343); // 0x015f000f
	retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 344); // 0x015f0700
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01570700
	snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01570503
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 348); // 0x016f0005
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 349); // 0x016f0009
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 350); // 0x016f000f
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 351); // 0x016f0012
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 352); // 0x016f000d
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 353); // 0x016f0009
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01670500
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 355); // 0x016f1c00
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 356); // 0x016f000c
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 357); // 0x016f000d
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 358); // 0x016f000f
	retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 359); // 0x016f0700
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01670700
	snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01670503
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 363); // 0x017f0005
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 364); // 0x017f0009
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 365); // 0x017f000f
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 366); // 0x017f0012
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 367); // 0x017f000d
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 368); // 0x017f0009
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01770500
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 370); // 0x017f1c00
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 371); // 0x017f000c
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 372); // 0x017f000d
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 373); // 0x017f000f
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 374); // 0x017f0700
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01770700
	snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01770503
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 378); // 0x018f0005
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 379); // 0x018f0009
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 380); // 0x018f000f
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 381); // 0x018f0012
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 382); // 0x018f000d
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 383); // 0x018f0009
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01870500
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x00ab9030, 385); // 0x018f1c00
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001724, 386); // 0x018f000c
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 387); // 0x018f000d
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 388); // 0x018f000f
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 389); // 0x018f0700
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01870700
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01870503
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 393); // 0x019f0005
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 394); // 0x019f0009
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 395); // 0x019f000f
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 396); // 0x019f0012
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 397); // 0x019f000d
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 398); // 0x019f0009
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90a60100, 399); // 0x019f1c00
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 400); // 0x019f000c
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 401); // 0x019f000d
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 402); // 0x019f0700
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01970700
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 405); // 0x01af0005
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 406); // 0x01af0009
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 407); // 0x01af000f
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 408); // 0x01af0012
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 409); // 0x01af000d
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 410); // 0x01af0009
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 411); // 0x01af1c00
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 412); // 0x01af000c
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 413); // 0x01af000d
	retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 414); // 0x01af0700
	snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01a70700
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 417); // 0x01bf0005
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 418); // 0x01bf0009
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 419); // 0x01bf000f
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 420); // 0x01bf0012
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 421); // 0x01bf000d
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 422); // 0x01bf0009
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 423); // 0x01bf1c00
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 424); // 0x01bf000c
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 425); // 0x01bf000d
	retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 426); // 0x01bf0700
	snd_hda_codec_write(codec, 0x1b, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01b70700
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 429); // 0x01cf0005
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 430); // 0x01cf0009
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 431); // 0x01cf000f
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 432); // 0x01cf0012
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 433); // 0x01cf000d
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 434); // 0x01cf0009
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 435); // 0x01cf1c00
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 436); // 0x01cf000c
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 437); // 0x01cf000d
	retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 438); // 0x01cf0700
	snd_hda_codec_write(codec, 0x1c, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01c70700
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 441); // 0x01df0005
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 442); // 0x01df0009
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 443); // 0x01df000f
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 444); // 0x01df0012
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 445); // 0x01df000d
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 446); // 0x01df0009
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90100110, 447); // 0x01df1c00
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 448); // 0x01df000c
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 449); // 0x01df000e
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000a, 450); // 0x01df0200
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 451); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 454); // 0x01ef0005
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 455); // 0x01ef0009
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 456); // 0x01ef000f
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 457); // 0x01ef0012
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 458); // 0x01ef000d
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 459); // 0x01ef0009
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 460); // 0x01ef1c00
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 461); // 0x01ef000c
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 462); // 0x01ef000e
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000b, 463); // 0x01ef0200
	retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 464); // 0x01ef0700
	snd_hda_codec_write(codec, 0x1e, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01e70700
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 467); // 0x01ff0005
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 468); // 0x01ff0009
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 469); // 0x01ff000f
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 470); // 0x01ff0012
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 471); // 0x01ff000d
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 472); // 0x01ff0009
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 473); // 0x01ff1c00
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 474); // 0x01ff000c
	retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 475); // 0x01ff0700
	snd_hda_codec_write(codec, 0x1f, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01f70700
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 478); // 0x020f0005
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 479); // 0x020f0009
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 480); // 0x020f000f
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 481); // 0x020f0012
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 482); // 0x020f000d
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 483); // 0x020f0009
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 484); // 0x020f1c00
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 485); // 0x020f000c
	retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 486); // 0x020f0700
	snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02070700
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 489); // 0x021f0005
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 490); // 0x021f0009
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 491); // 0x021f000f
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 492); // 0x021f0012
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 493); // 0x021f000d
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 494); // 0x021f0009
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 495); // 0x021f1c00
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000014, 496); // 0x021f000c
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 497); // 0x021f000e
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000e, 498); // 0x021f0200
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 499); // 0x021f0700
	snd_hda_codec_write(codec, 0x21, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02170700
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 502); // 0x022f0005
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 503); // 0x022f0009
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 504); // 0x022f000f
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 505); // 0x022f0012
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 506); // 0x022f000d
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 507); // 0x022f0009
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 508); // 0x022f1c00
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 509); // 0x022f000c
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 510); // 0x022f0700
	snd_hda_codec_write(codec, 0x22, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02270700
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 513); // 0x023f0005
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 514); // 0x023f0009
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 515); // 0x023f000f
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 516); // 0x023f0012
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 517); // 0x023f000d
	retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 518); // 0x023f0009
	snd_hda_codec_write(codec, 0x23, 0, AC_VERB_SET_BEEP_CONTROL, 0x00000000); // 0x02370a00
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 521); // 0x024f0005
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 522); // 0x024f0009
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 523); // 0x024f000f
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 524); // 0x024f0012
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 525); // 0x024f000d
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 526); // 0x024f0009
	retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000010, 0x00008000, 527); // 0x024f0010
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 1067); // 0x025f0005
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1068); // 0x025f0009
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 1069); // 0x025f000f
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 1070); // 0x025f000a
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 1071); // 0x025f000b
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 1072); // 0x025f0012
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 1073); // 0x025f000d
	retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1074); // 0x025f0009
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000004); // 0x00171604
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000004, 1102); // 0x001f1500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000006); // 0x00171606
	retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000006, 1105); // 0x001f1500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000020); // 0x00171720
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000026); // 0x00171626
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000030); // 0x00171730
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000036); // 0x00171636
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1174); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1175); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1176); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1177); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1178); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1179); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1227); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1230); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1232); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883
	snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000009); // 0x01870809
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1259); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 1262); // 0x006f0500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00670600
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1265); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1276); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1278); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1280); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1282); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1284); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1286); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1288); // 0x019b2000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1290); // 0x019b0000
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
	retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1292); // 0x019f0700
	snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01970720
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1319); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060b3); // 0x006360b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1321); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050b3); // 0x006350b3
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1324); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1326); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1335); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1337); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1340); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1343); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1344); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1347); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1348); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1369); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1370); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1371); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1372); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1373); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1374); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1375); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1376); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1394); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1397); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1398); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1402); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1403); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1406); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1409); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1410); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1414); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1415); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1418); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1421); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1422); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1426); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1427); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1429); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1432); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1433); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1437); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1438); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1443); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1461); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1464); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1465); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1469); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1470); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1473); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1475); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1478); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1480); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1501); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1502); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1503); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1504); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1505); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1506); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1507); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1508); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1526); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1529); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1530); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1533); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1534); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1537); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1540); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1541); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1545); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1546); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1549); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1552); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1553); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1557); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1558); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1560); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1563); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1564); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1568); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1569); // 0x00df0d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1572); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1574); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1577); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1580); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1581); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1584); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1585); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1606); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1607); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1608); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1609); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1610); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1611); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1612); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1613); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1631); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1634); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1635); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1639); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1640); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1643); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1646); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1647); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1651); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1652); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1655); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1658); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1659); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1663); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1664); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1666); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1669); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1670); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1674); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1675); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1680); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1710); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1713); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1714); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1718); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1719); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1722); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1724); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1733); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1735); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1744); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1746); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1785); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1786); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1789); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1790); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1791); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1792); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1793); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1794); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1800); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1803); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1805); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1847); // 0x010f0900
	retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1848); // 0x011f0900
	retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1849); // 0x017f0900
	retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1850); // 0x018f0900
	retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1851); // 0x021f0900
	retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_SENSE, 0x00000000, 0x00000000, 1852); // 0x022f0900
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1858); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1861); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1863); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1917); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1920); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1922); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1931); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1933); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1942); // 0x006b2000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1944); // 0x006b0000
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2009); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2015); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2017); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2020); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 2026); // 0x010f0500
	snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503
	retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2028); // 0x010f0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2038); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 2039); // 0x00af0500
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

	codec_dbg(codec, "setup_a1534 end");
	return 0;
}

static int play_a1534 (struct hda_codec *codec) {
        int retval;
	codec_dbg(codec, "play_a1534 begin");

	retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 9); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 10); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 14); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 17); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 21); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 41); // 0x00af0600
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 42); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 44); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 45); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 48); // 0x00bf0600
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 49); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 50); // 0x00cf0600
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 51); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 52); // 0x00df0600
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 53); // 0x00df0500
	snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 73); // 0x00af0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 74); // 0x00af0d00
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 76); // 0x00af0500
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 78); // 0x00bf0500
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 81); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 82); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 86); // 0x00bf0500
	retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 87); // 0x00bf0d00
	snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00b70d00
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 90); // 0x00cf0500
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 93); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 94); // 0x00cf0d00
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00c70d01
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00c70600
	snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 98); // 0x00cf0500
	retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 99); // 0x00cf0d00
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 101); // 0x00df0500
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 104); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 105); // 0x00df0d00
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
	snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 109); // 0x00df0500
	retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 110); // 0x00df0d00
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01
	snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11
	retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 115); // 0x01df0700
	snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
	snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
	retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 129); // 0x006f0500
	retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 130); // 0x00af0500

	codec_dbg(codec, "play_a1534 end");
	return 0;
}

#include "patch_cirrus_a1534_pcm.h"
