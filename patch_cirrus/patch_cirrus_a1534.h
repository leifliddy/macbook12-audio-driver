// credits to davidjo for making speakers work on mbp14,3
// the below is setup for mb9,1

//this line only needed for kernel 5.0 and below
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
static int headphones_a1534 (struct hda_codec *codec) {
	int retval;
	//printk("snd_hda_intel: headphones_a1534 begin");
id

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

	//printk("snd_hda_intel: headphones_a1534 end");
}
static int setup_a1534 (struct hda_codec *codec) {
	int retval;
	//printk("snd_hda_intel: setup_a1534 begin");

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

//        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_DBL_CODEC_RESET, 0x00000000); // 0x001fff00

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 13); // 0x000f0000
        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000002, 0x00100401, 14); // 0x000f0002
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000101, 15); // 0x001f0005
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000f, 0xe0000019, 16); // 0x001f000f
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 17); // 0x001f000a
//       snd_hda:     pcm params           1 bits:  rates: 
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 18); // 0x001f000b
//       snd_hda:     stream format params 1 
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 19); // 0x001f0012
//       snd_hda:     amp capabilities 1 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 20); // 0x001f000d
//       snd_hda:     amp capabilities 1 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_SUBSYSTEM_ID, 0x00000000, 0x106b6500, 22); // 0x001f2000

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000008, 0x00010000, 23); // 0x001f0008

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DIRECTION, 0x00000000, 0x00000000, 24); // 0x001f1700
//       snd_hda:     gpio direction 1 0x00 in in in in in in in in

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000011, 0xc0000206, 25); // 0x001f0011
//       snd_hda:     gpio params 1 [('GPIO', 6), ('GPIO_WAKE', 1), ('GPO', 2), ('GPI', 0), ('GPIO_UNSOL', 1)]
        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_PARAMETERS, 0x00000004, 0x00020023, 26); // 0x001f0004
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 27); // 0x002f0005
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 28); // 0x002f0009
//       snd_hda:     2 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 29); // 0x002f000f
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 30); // 0x002f000a
//       snd_hda:     pcm params           2 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 31); // 0x002f000b
//       snd_hda:     stream format params 2 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 32); // 0x002f0012
//       snd_hda:     amp capabilities 2 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 33); // 0x002f000d
//       snd_hda:     amp capabilities 2 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 34); // 0x002f0009
//       snd_hda:     2 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00270500

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 36); // 0x002f000a
//       snd_hda:     pcm params           2 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 37); // 0x002f000b
//       snd_hda:     stream format params 2 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 38); // 0x002f0012
//       snd_hda:     amp capabilities 2 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 39); // 0x002f000f

        retval = snd_hda_codec_read_check(codec, 0x02, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 40); // 0x002f2400

        snd_hda_codec_write(codec, 0x02, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00270503


        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 43); // 0x003f0005
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 44); // 0x003f0009
//       snd_hda:     3 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 45); // 0x003f000f
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 46); // 0x003f000a
//       snd_hda:     pcm params           3 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 47); // 0x003f000b
//       snd_hda:     stream format params 3 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 48); // 0x003f0012
//       snd_hda:     amp capabilities 3 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 49); // 0x003f000d
//       snd_hda:     amp capabilities 3 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 50); // 0x003f0009
//       snd_hda:     3 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0

        snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00370500

        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 52); // 0x003f000a
//       snd_hda:     pcm params           3 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 53); // 0x003f000b
//       snd_hda:     stream format params 3 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 54); // 0x003f0012
//       snd_hda:     amp capabilities 3 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 55); // 0x003f000f

        retval = snd_hda_codec_read_check(codec, 0x03, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 56); // 0x003f2400

        snd_hda_codec_write(codec, 0x03, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00370503


        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 59); // 0x004f0005
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 60); // 0x004f0009
//       snd_hda:     4 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 61); // 0x004f000f
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 62); // 0x004f000a
//       snd_hda:     pcm params           4 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 63); // 0x004f000b
//       snd_hda:     stream format params 4 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 64); // 0x004f0012
//       snd_hda:     amp capabilities 4 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 65); // 0x004f000d
//       snd_hda:     amp capabilities 4 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 66); // 0x004f0009
//       snd_hda:     4 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0

        snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00470500

        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 68); // 0x004f000a
//       snd_hda:     pcm params           4 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 69); // 0x004f000b
//       snd_hda:     stream format params 4 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 70); // 0x004f0012
//       snd_hda:     amp capabilities 4 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 71); // 0x004f000f

        retval = snd_hda_codec_read_check(codec, 0x04, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 72); // 0x004f2400

        snd_hda_codec_write(codec, 0x04, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00470503


        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 75); // 0x005f0005
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 76); // 0x005f0009
//       snd_hda:     5 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 77); // 0x005f000f
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 78); // 0x005f000a
//       snd_hda:     pcm params           5 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 79); // 0x005f000b
//       snd_hda:     stream format params 5 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 80); // 0x005f0012
//       snd_hda:     amp capabilities 5 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 81); // 0x005f000d
//       snd_hda:     amp capabilities 5 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000009, 0x000d043d, 82); // 0x005f0009
//       snd_hda:     5 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_OUT_AMP', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT'] 0 13 0

        snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00570500

        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e07f0, 84); // 0x005f000a
//       snd_hda:     pcm params           5 bits: 16bit 20bit 24bit 32bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 85); // 0x005f000b
//       snd_hda:     stream format params 5 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80017f7f, 86); // 0x005f0012
//       snd_hda:     amp capabilities 5 output 0x80017f7f offset 0x7f numsteps 0x7f stepsize 0x01 mute 1
        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 87); // 0x005f000f

        retval = snd_hda_codec_read_check(codec, 0x05, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 88); // 0x005f2400

        snd_hda_codec_write(codec, 0x05, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00570503


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 91); // 0x006f0005
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 92); // 0x006f0009
//       snd_hda:     6 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 93); // 0x006f000f
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 94); // 0x006f000a
//       snd_hda:     pcm params           6 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 95); // 0x006f000b
//       snd_hda:     stream format params 6 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 96); // 0x006f0012
//       snd_hda:     amp capabilities 6 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 97); // 0x006f000d
//       snd_hda:     amp capabilities 6 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 98); // 0x006f0009
//       snd_hda:     6 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 100); // 0x006f000a
//       snd_hda:     pcm params           6 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 101); // 0x006f000b
//       snd_hda:     stream format params 6 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 102); // 0x006f000d
//       snd_hda:     amp capabilities 6 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 103); // 0x006f000e

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00191715, 104); // 0x006f0200
//       snd_hda:     connection list 6 <- 21 23 25

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 105); // 0x006f000f

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503


        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 108); // 0x007f0005
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 109); // 0x007f0009
//       snd_hda:     7 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 110); // 0x007f000f
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 111); // 0x007f000a
//       snd_hda:     pcm params           7 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 112); // 0x007f000b
//       snd_hda:     stream format params 7 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 113); // 0x007f0012
//       snd_hda:     amp capabilities 7 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 114); // 0x007f000d
//       snd_hda:     amp capabilities 7 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 115); // 0x007f0009
//       snd_hda:     7 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1

        snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00770500

        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 117); // 0x007f000a
//       snd_hda:     pcm params           7 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 118); // 0x007f000b
//       snd_hda:     stream format params 7 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 119); // 0x007f000d
//       snd_hda:     amp capabilities 7 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000003, 120); // 0x007f000e

        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x001a1816, 121); // 0x007f0200
//       snd_hda:     connection list 7 <- 22 24 26

        retval = snd_hda_codec_read_check(codec, 0x07, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 122); // 0x007f000f

        snd_hda_codec_write(codec, 0x07, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00770503


        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 125); // 0x008f0005
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 126); // 0x008f0009
//       snd_hda:     8 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 127); // 0x008f000f
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 128); // 0x008f000a
//       snd_hda:     pcm params           8 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 129); // 0x008f000b
//       snd_hda:     stream format params 8 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 130); // 0x008f0012
//       snd_hda:     amp capabilities 8 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 131); // 0x008f000d
//       snd_hda:     amp capabilities 8 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 132); // 0x008f0009
//       snd_hda:     8 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1

        snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00870500

        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 134); // 0x008f000a
//       snd_hda:     pcm params           8 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 135); // 0x008f000b
//       snd_hda:     stream format params 8 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 136); // 0x008f000d
//       snd_hda:     amp capabilities 8 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 137); // 0x008f000e

        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001b, 138); // 0x008f0200
//       snd_hda:     connection list 8 <- 27

        retval = snd_hda_codec_read_check(codec, 0x08, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 139); // 0x008f000f

        snd_hda_codec_write(codec, 0x08, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00870503


        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 142); // 0x009f0005
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 143); // 0x009f0009
//       snd_hda:     9 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 144); // 0x009f000f
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 145); // 0x009f000a
//       snd_hda:     pcm params           9 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 146); // 0x009f000b
//       snd_hda:     stream format params 9 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 147); // 0x009f0012
//       snd_hda:     amp capabilities 9 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 148); // 0x009f000d
//       snd_hda:     amp capabilities 9 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0018051b, 149); // 0x009f0009
//       snd_hda:     9 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 8 1

        snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00970500

        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e01f5, 151); // 0x009f000a
//       snd_hda:     pcm params           9 bits: 16bit 20bit 24bit 32bit rates: 8kHz 16kHz 32kHz 44.1kHz 48kHz 88.2kHz 96kHz
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000003, 152); // 0x009f000b
//       snd_hda:     stream format params 9 float32 pcm
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x80033f33, 153); // 0x009f000d
//       snd_hda:     amp capabilities 9 input  0x80033f33 offset 0x33 numsteps 0x3f stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 154); // 0x009f000e

        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001c, 155); // 0x009f0200
//       snd_hda:     connection list 9 <- 28

        retval = snd_hda_codec_read_check(codec, 0x09, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 156); // 0x009f000f

        snd_hda_codec_write(codec, 0x09, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00970503


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 159); // 0x00af0005
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 160); // 0x00af0009
//       snd_hda:     10 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 4 0
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 161); // 0x00af000f
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 162); // 0x00af000a
//       snd_hda:     pcm params           10 bits: 16bit 20bit 24bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 163); // 0x00af000b
//       snd_hda:     stream format params 10 pcm
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 164); // 0x00af0012
//       snd_hda:     amp capabilities 10 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 165); // 0x00af000d
//       snd_hda:     amp capabilities 10 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 166); // 0x00af0009
//       snd_hda:     10 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 4 0

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 168); // 0x00af000a
//       snd_hda:     pcm params           10 bits: 16bit 20bit 24bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 169); // 0x00af000b
//       snd_hda:     stream format params 10 pcm
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 170); // 0x00af000f

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 171); // 0x00af2400

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503


        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 174); // 0x00bf0005
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 175); // 0x00bf0009
//       snd_hda:     11 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 4 0
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 176); // 0x00bf000f
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 177); // 0x00bf000a
//       snd_hda:     pcm params           11 bits: 16bit 20bit 24bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 178); // 0x00bf000b
//       snd_hda:     stream format params 11 pcm
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 179); // 0x00bf0012
//       snd_hda:     amp capabilities 11 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 180); // 0x00bf000d
//       snd_hda:     amp capabilities 11 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00046631, 181); // 0x00bf0009
//       snd_hda:     11 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 4 0

        snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e0060, 183); // 0x00bf000a
//       snd_hda:     pcm params           11 bits: 16bit 20bit 24bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 184); // 0x00bf000b
//       snd_hda:     stream format params 11 pcm
        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 185); // 0x00bf000f

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 186); // 0x00bf2400

        snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00b70503


        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 189); // 0x00cf0005
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 190); // 0x00cf0009
//       snd_hda:     12 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 3 1
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 191); // 0x00cf000f
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 192); // 0x00cf000a
//       snd_hda:     pcm params           12 bits: 16bit 20bit 24bit 32bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 193); // 0x00cf000b
//       snd_hda:     stream format params 12 pcm
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 194); // 0x00cf0012
//       snd_hda:     amp capabilities 12 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 195); // 0x00cf000d
//       snd_hda:     amp capabilities 12 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 196); // 0x00cf0009
//       snd_hda:     12 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 3 1

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00c70500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 198); // 0x00cf000a
//       snd_hda:     pcm params           12 bits: 16bit 20bit 24bit 32bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 199); // 0x00cf000b
//       snd_hda:     stream format params 12 pcm
        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 200); // 0x00cf000e

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000001f, 201); // 0x00cf0200
//       snd_hda:     connection list 12 <- 31

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 202); // 0x00cf000f

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503


        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 205); // 0x00df0005
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 206); // 0x00df0009
//       snd_hda:     13 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 3 1
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 207); // 0x00df000f
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 208); // 0x00df000a
//       snd_hda:     pcm params           13 bits: 16bit 20bit 24bit 32bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 209); // 0x00df000b
//       snd_hda:     stream format params 13 pcm
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 210); // 0x00df0012
//       snd_hda:     amp capabilities 13 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 211); // 0x00df000d
//       snd_hda:     amp capabilities 13 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00136711, 212); // 0x00df0009
//       snd_hda:     13 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 3 1

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x001e0060, 214); // 0x00df000a
//       snd_hda:     pcm params           13 bits: 16bit 20bit 24bit 32bit rates: 44.1kHz 48kHz
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000001, 215); // 0x00df000b
//       snd_hda:     stream format params 13 pcm
        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 216); // 0x00df000e

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000020, 217); // 0x00df0200
//       snd_hda:     connection list 13 <- 32

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 218); // 0x00df000f

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503


        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 221); // 0x00ef0005
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 222); // 0x00ef0009
//       snd_hda:     14 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 4 0
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 223); // 0x00ef000f
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 224); // 0x00ef000a
//       snd_hda:     pcm params           14 bits: 16bit 20bit 24bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 225); // 0x00ef000b
//       snd_hda:     stream format params 14 ac3 pcm
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 226); // 0x00ef0012
//       snd_hda:     amp capabilities 14 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 227); // 0x00ef000d
//       snd_hda:     amp capabilities 14 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00040631, 228); // 0x00ef0009
//       snd_hda:     14 AC_WID_AUD_OUT ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_STRIPE', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 4 0

        snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00e70500

        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e07f0, 230); // 0x00ef000a
//       snd_hda:     pcm params           14 bits: 16bit 20bit 24bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 176.4kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 231); // 0x00ef000b
//       snd_hda:     stream format params 14 ac3 pcm
        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 232); // 0x00ef000f

        retval = snd_hda_codec_read_check(codec, 0x0e, 0, AC_VERB_GET_STRIPE_CONTROL, 0x00000000, 0x00100000, 233); // 0x00ef2400

        snd_hda_codec_write(codec, 0x0e, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00e70503


        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 236); // 0x00ff0005
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 237); // 0x00ff0009
//       snd_hda:     15 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 11 1
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 238); // 0x00ff000f
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 239); // 0x00ff000a
//       snd_hda:     pcm params           15 bits: 16bit 20bit 24bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 240); // 0x00ff000b
//       snd_hda:     stream format params 15 ac3 pcm
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 241); // 0x00ff0012
//       snd_hda:     amp capabilities 15 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 242); // 0x00ff000d
//       snd_hda:     amp capabilities 15 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x001b0791, 243); // 0x00ff0009
//       snd_hda:     15 AC_WID_AUD_IN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_FORMAT_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 11 1

        snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00f70500

        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x000e05f0, 245); // 0x00ff000a
//       snd_hda:     pcm params           15 bits: 16bit 20bit 24bit rates: 32kHz 44.1kHz 48kHz 88.2kHz 96kHz 192kHz
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000005, 246); // 0x00ff000b
//       snd_hda:     stream format params 15 ac3 pcm
        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 247); // 0x00ff000e

        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000022, 248); // 0x00ff0200
//       snd_hda:     connection list 15 <- 34

        retval = snd_hda_codec_read_check(codec, 0x0f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 249); // 0x00ff000f

        snd_hda_codec_write(codec, 0x0f, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00f70503


        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 252); // 0x010f0005
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 253); // 0x010f0009
//       snd_hda:     16 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_OUT_AMP', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_POWER', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 254); // 0x010f000f
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 255); // 0x010f0012
//       snd_hda:     amp capabilities 16 output 0x80034242 offset 0x42 numsteps 0x42 stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 256); // 0x010f000d
//       snd_hda:     amp capabilities 16 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040058d, 257); // 0x010f0009
//       snd_hda:     16 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_OUT_AMP', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_POWER', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01070500

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x002b4020, 259); // 0x010f1c00
//       snd_hda:     pin config default 0x002b4020 port conn 0 location 0x0 loc ext 0 loc geom 0 default device 2 conn type 11 color 4 misc 0 def assoc 2 seq 0
//       snd_hda:     pin config default 0x002b4020 port conn Jack loc ext Ext loc geom N/A default device HP Out conn type Combination color Green misc undef def assoc 2 seq 0

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x0000001c, 260); // 0x010f000c
//       snd_hda:     16 ['AC_PINCAP_OUT', 'AC_PINCAP_PRES_DETECT', 'AC_PINCAP_HP_DRV'] []
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x00000012, 0x80034242, 261); // 0x010f0012
//       snd_hda:     amp capabilities 16 output 0x80034242 offset 0x42 numsteps 0x42 stepsize 0x03 mute 1
        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 262); // 0x010f000e

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 263); // 0x010f0200
//       snd_hda:     connection list 16 <- 2

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 264); // 0x010f000f

        retval = snd_hda_codec_read_check(codec, 0x10, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 265); // 0x010f0700
        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01070700
//       snd_hda:     16 []

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01070503


        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 269); // 0x011f0005
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 270); // 0x011f0009
//       snd_hda:     17 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 271); // 0x011f000f
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 272); // 0x011f0012
//       snd_hda:     amp capabilities 17 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 273); // 0x011f000d
//       snd_hda:     amp capabilities 17 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400581, 274); // 0x011f0009
//       snd_hda:     17 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4

        snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01170500

        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 276); // 0x011f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000054, 277); // 0x011f000c
//       snd_hda:     17 ['AC_PINCAP_BALANCE', 'AC_PINCAP_OUT', 'AC_PINCAP_PRES_DETECT'] []
        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 278); // 0x011f000e

        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000002, 279); // 0x011f0200
//       snd_hda:     connection list 17 <- 2

        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 280); // 0x011f000f

        retval = snd_hda_codec_read_check(codec, 0x11, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 281); // 0x011f0700
        snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01170700
//       snd_hda:     17 []

        snd_hda_codec_write(codec, 0x11, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01170503


        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 285); // 0x012f0005
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 286); // 0x012f0009
//       snd_hda:     18 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 287); // 0x012f000f
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 288); // 0x012f0012
//       snd_hda:     amp capabilities 18 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 289); // 0x012f000d
//       snd_hda:     amp capabilities 18 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 290); // 0x012f0009
//       snd_hda:     18 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4

        snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01270500

        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 292); // 0x012f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 293); // 0x012f000c
//       snd_hda:     18 ['AC_PINCAP_OUT', 'AC_PINCAP_BALANCE'] []
        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 294); // 0x012f000e

        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000003, 295); // 0x012f0200
//       snd_hda:     connection list 18 <- 3

        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 296); // 0x012f000f

        retval = snd_hda_codec_read_check(codec, 0x12, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 297); // 0x012f0700
        snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01270700
//       snd_hda:     18 []

        snd_hda_codec_write(codec, 0x12, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01270503


        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 301); // 0x013f0005
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 302); // 0x013f0009
//       snd_hda:     19 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 303); // 0x013f000f
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 304); // 0x013f0012
//       snd_hda:     amp capabilities 19 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 305); // 0x013f000d
//       snd_hda:     amp capabilities 19 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 306); // 0x013f0009
//       snd_hda:     19 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4

        snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01370500

        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 308); // 0x013f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 309); // 0x013f000c
//       snd_hda:     19 ['AC_PINCAP_OUT', 'AC_PINCAP_BALANCE'] []
        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 310); // 0x013f000e

        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000004, 311); // 0x013f0200
//       snd_hda:     connection list 19 <- 4

        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 312); // 0x013f000f

        retval = snd_hda_codec_read_check(codec, 0x13, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 313); // 0x013f0700
        snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01370700
//       snd_hda:     19 []

        snd_hda_codec_write(codec, 0x13, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01370503


        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 317); // 0x014f0005
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 318); // 0x014f0009
//       snd_hda:     20 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 319); // 0x014f000f
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 320); // 0x014f0012
//       snd_hda:     amp capabilities 20 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 321); // 0x014f000d
//       snd_hda:     amp capabilities 20 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400501, 322); // 0x014f0009
//       snd_hda:     20 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT'] 0 0 4

        snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01470500

        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 324); // 0x014f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000050, 325); // 0x014f000c
//       snd_hda:     20 ['AC_PINCAP_OUT', 'AC_PINCAP_BALANCE'] []
        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 326); // 0x014f000e

        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x00000005, 327); // 0x014f0200
//       snd_hda:     connection list 20 <- 5

        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 328); // 0x014f000f

        retval = snd_hda_codec_read_check(codec, 0x14, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 329); // 0x014f0700
        snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01470700
//       snd_hda:     20 []

        snd_hda_codec_write(codec, 0x14, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01470503


        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 333); // 0x015f0005
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 334); // 0x015f0009
//       snd_hda:     21 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 335); // 0x015f000f
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 336); // 0x015f0012
//       snd_hda:     amp capabilities 21 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 337); // 0x015f000d
//       snd_hda:     amp capabilities 21 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 338); // 0x015f0009
//       snd_hda:     21 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01570500

        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 340); // 0x015f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 341); // 0x015f000c
//       snd_hda:     21 ['AC_PINCAP_IN'] ['AC_PINCAP_VREF_GRD', 'AC_PINCAP_VREF_HIZ', 'AC_PINCAP_VREF_50']
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 342); // 0x015f000d
//       snd_hda:     amp capabilities 21 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 343); // 0x015f000f

        retval = snd_hda_codec_read_check(codec, 0x15, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 344); // 0x015f0700
        snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01570700
//       snd_hda:     21 []

        snd_hda_codec_write(codec, 0x15, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01570503


        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 348); // 0x016f0005
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 349); // 0x016f0009
//       snd_hda:     22 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 350); // 0x016f000f
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 351); // 0x016f0012
//       snd_hda:     amp capabilities 22 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 352); // 0x016f000d
//       snd_hda:     amp capabilities 22 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040040b, 353); // 0x016f0009
//       snd_hda:     22 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01670500

        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 355); // 0x016f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001720, 356); // 0x016f000c
//       snd_hda:     22 ['AC_PINCAP_IN'] ['AC_PINCAP_VREF_GRD', 'AC_PINCAP_VREF_HIZ', 'AC_PINCAP_VREF_50']
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 357); // 0x016f000d
//       snd_hda:     amp capabilities 22 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 358); // 0x016f000f

        retval = snd_hda_codec_read_check(codec, 0x16, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 359); // 0x016f0700
        snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01670700
//       snd_hda:     22 []

        snd_hda_codec_write(codec, 0x16, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01670503


        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 363); // 0x017f0005
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 364); // 0x017f0009
//       snd_hda:     23 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 365); // 0x017f000f
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 366); // 0x017f0012
//       snd_hda:     amp capabilities 23 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 367); // 0x017f000d
//       snd_hda:     amp capabilities 23 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048b, 368); // 0x017f0009
//       snd_hda:     23 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01770500

        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 370); // 0x017f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 371); // 0x017f000c
//       snd_hda:     23 ['AC_PINCAP_IN', 'AC_PINCAP_PRES_DETECT'] []
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 372); // 0x017f000d
//       snd_hda:     amp capabilities 23 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 373); // 0x017f000f

        retval = snd_hda_codec_read_check(codec, 0x17, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 374); // 0x017f0700
        snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01770700
//       snd_hda:     23 []

        snd_hda_codec_write(codec, 0x17, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01770503


        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 378); // 0x018f0005
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 379); // 0x018f0009
//       snd_hda:     24 AC_WID_PIN ['AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 380); // 0x018f000f
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 381); // 0x018f0012
//       snd_hda:     amp capabilities 24 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 382); // 0x018f000d
//       snd_hda:     amp capabilities 24 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040048a, 383); // 0x018f0009
//       snd_hda:     24 AC_WID_PIN ['AC_WCAP_POWER', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x01870500

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x00ab9030, 385); // 0x018f1c00
//       snd_hda:     pin config default 0x00ab9030 port conn 0 location 0x0 loc ext 0 loc geom 0 default device 10 conn type 11 color 9 misc 0 def assoc 3 seq 0
//       snd_hda:     pin config default 0x00ab9030 port conn Jack loc ext Ext loc geom N/A default device Mic In conn type Combination color Pink misc undef def assoc 3 seq 0

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00001724, 386); // 0x018f000c
//       snd_hda:     24 ['AC_PINCAP_IN', 'AC_PINCAP_PRES_DETECT'] ['AC_PINCAP_VREF_GRD', 'AC_PINCAP_VREF_HIZ', 'AC_PINCAP_VREF_50']
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270300, 387); // 0x018f000d
//       snd_hda:     amp capabilities 24 input  0x00270300 offset 0x00 numsteps 0x03 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x80000009, 388); // 0x018f000f

        retval = snd_hda_codec_read_check(codec, 0x18, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 389); // 0x018f0700
        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01870700
//       snd_hda:     24 []

        snd_hda_codec_write(codec, 0x18, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x01870503


        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 393); // 0x019f0005
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 394); // 0x019f0009
//       snd_hda:     25 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 395); // 0x019f000f
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 396); // 0x019f0012
//       snd_hda:     amp capabilities 25 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 397); // 0x019f000d
//       snd_hda:     amp capabilities 25 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 398); // 0x019f0009
//       snd_hda:     25 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90a60100, 399); // 0x019f1c00
//       snd_hda:     pin config default 0x90a60100 port conn 2 location 0x10 loc ext 1 loc geom 0 default device 10 conn type 6 color 0 misc 1 def assoc 0 seq 0
//       snd_hda:     pin config default 0x90a60100 port conn Fixed loc ext Int loc geom N/A default device Mic In conn type Other Digital color Unknown misc Jack Detect Override def assoc 0 seq 0

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 400); // 0x019f000c
//       snd_hda:     25 ['AC_PINCAP_IN'] []
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 401); // 0x019f000d
//       snd_hda:     amp capabilities 25 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 402); // 0x019f0700
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01970700
//       snd_hda:     25 []


        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 405); // 0x01af0005
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 406); // 0x01af0009
//       snd_hda:     26 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 407); // 0x01af000f
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 408); // 0x01af0012
//       snd_hda:     amp capabilities 26 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 409); // 0x01af000d
//       snd_hda:     amp capabilities 26 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 410); // 0x01af0009
//       snd_hda:     26 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 411); // 0x01af1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 412); // 0x01af000c
//       snd_hda:     26 ['AC_PINCAP_IN'] []
        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 413); // 0x01af000d
//       snd_hda:     amp capabilities 26 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0

        retval = snd_hda_codec_read_check(codec, 0x1a, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 414); // 0x01af0700
        snd_hda_codec_write(codec, 0x1a, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01a70700
//       snd_hda:     26 []


        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 417); // 0x01bf0005
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 418); // 0x01bf0009
//       snd_hda:     27 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 419); // 0x01bf000f
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 420); // 0x01bf0012
//       snd_hda:     amp capabilities 27 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 421); // 0x01bf000d
//       snd_hda:     amp capabilities 27 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 422); // 0x01bf0009
//       snd_hda:     27 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 423); // 0x01bf1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 424); // 0x01bf000c
//       snd_hda:     27 ['AC_PINCAP_IN'] []
        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 425); // 0x01bf000d
//       snd_hda:     amp capabilities 27 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0

        retval = snd_hda_codec_read_check(codec, 0x1b, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 426); // 0x01bf0700
        snd_hda_codec_write(codec, 0x1b, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01b70700
//       snd_hda:     27 []


        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 429); // 0x01cf0005
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 430); // 0x01cf0009
//       snd_hda:     28 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 431); // 0x01cf000f
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 432); // 0x01cf0012
//       snd_hda:     amp capabilities 28 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 433); // 0x01cf000d
//       snd_hda:     amp capabilities 28 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x00000009, 0x0040000b, 434); // 0x01cf0009
//       snd_hda:     28 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_AMP_OVRD', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_IN_AMP'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 435); // 0x01cf1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 436); // 0x01cf000c
//       snd_hda:     28 ['AC_PINCAP_IN'] []
        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00270200, 437); // 0x01cf000d
//       snd_hda:     amp capabilities 28 input  0x00270200 offset 0x00 numsteps 0x02 stepsize 0x27 mute 0

        retval = snd_hda_codec_read_check(codec, 0x1c, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 438); // 0x01cf0700
        snd_hda_codec_write(codec, 0x1c, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01c70700
//       snd_hda:     28 []


        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 441); // 0x01df0005
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 442); // 0x01df0009
//       snd_hda:     29 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 0 4
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 443); // 0x01df000f
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 444); // 0x01df0012
//       snd_hda:     amp capabilities 29 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 445); // 0x01df000d
//       snd_hda:     amp capabilities 29 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 446); // 0x01df0009
//       snd_hda:     29 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 0 4

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x90100110, 447); // 0x01df1c00
//       snd_hda:     pin config default 0x90100110 port conn 2 location 0x10 loc ext 1 loc geom 0 default device 1 conn type 0 color 0 misc 1 def assoc 1 seq 0
//       snd_hda:     pin config default 0x90100110 port conn Fixed loc ext Int loc geom N/A default device Speaker conn type Unknown color Unknown misc Jack Detect Override def assoc 1 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 448); // 0x01df000c
//       snd_hda:     29 ['AC_PINCAP_OUT'] []
        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 449); // 0x01df000e

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000a, 450); // 0x01df0200
//       snd_hda:     connection list 29 <- 10

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 451); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
//       snd_hda:     29 []


        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 454); // 0x01ef0005
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 455); // 0x01ef0009
//       snd_hda:     30 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 0 4
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 456); // 0x01ef000f
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 457); // 0x01ef0012
//       snd_hda:     amp capabilities 30 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 458); // 0x01ef000d
//       snd_hda:     amp capabilities 30 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406301, 459); // 0x01ef0009
//       snd_hda:     30 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 3 0 4

        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 460); // 0x01ef1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000010, 461); // 0x01ef000c
//       snd_hda:     30 ['AC_PINCAP_OUT'] []
        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 462); // 0x01ef000e

        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000b, 463); // 0x01ef0200
//       snd_hda:     connection list 30 <- 11

        retval = snd_hda_codec_read_check(codec, 0x1e, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 464); // 0x01ef0700
        snd_hda_codec_write(codec, 0x1e, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01e70700
//       snd_hda:     30 []


        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 467); // 0x01ff0005
        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 468); // 0x01ff0009
//       snd_hda:     31 AC_WID_PIN ['AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_STEREO', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 3 0 4
        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 469); // 0x01ff000f
        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 470); // 0x01ff0012
//       snd_hda:     amp capabilities 31 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 471); // 0x01ff000d
//       snd_hda:     amp capabilities 31 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 472); // 0x01ff0009
//       snd_hda:     31 AC_WID_PIN ['AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_STEREO', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 3 0 4

        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 473); // 0x01ff1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 474); // 0x01ff000c
//       snd_hda:     31 ['AC_PINCAP_IN'] []

        retval = snd_hda_codec_read_check(codec, 0x1f, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 475); // 0x01ff0700
        snd_hda_codec_write(codec, 0x1f, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01f70700
//       snd_hda:     31 []


        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 478); // 0x020f0005
        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 479); // 0x020f0009
//       snd_hda:     32 AC_WID_PIN ['AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_STEREO', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 3 0 4
        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 480); // 0x020f000f
        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 481); // 0x020f0012
//       snd_hda:     amp capabilities 32 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 482); // 0x020f000d
//       snd_hda:     amp capabilities 32 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00406201, 483); // 0x020f0009
//       snd_hda:     32 AC_WID_PIN ['AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_STEREO', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 3 0 4

        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 484); // 0x020f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000020, 485); // 0x020f000c
//       snd_hda:     32 ['AC_PINCAP_IN'] []

        retval = snd_hda_codec_read_check(codec, 0x20, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 486); // 0x020f0700
        snd_hda_codec_write(codec, 0x20, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02070700
//       snd_hda:     32 []


        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 489); // 0x021f0005
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 490); // 0x021f0009
//       snd_hda:     33 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 491); // 0x021f000f
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 492); // 0x021f0012
//       snd_hda:     amp capabilities 33 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 493); // 0x021f000d
//       snd_hda:     amp capabilities 33 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400381, 494); // 0x021f0009
//       snd_hda:     33 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_CONN_LIST', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 495); // 0x021f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000014, 496); // 0x021f000c
//       snd_hda:     33 ['AC_PINCAP_OUT', 'AC_PINCAP_PRES_DETECT'] []
        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_PARAMETERS, 0x0000000e, 0x00000001, 497); // 0x021f000e

        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_CONNECT_LIST, 0x00000000, 0x0000000e, 498); // 0x021f0200
//       snd_hda:     connection list 33 <- 14

        retval = snd_hda_codec_read_check(codec, 0x21, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 499); // 0x021f0700
        snd_hda_codec_write(codec, 0x21, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02170700
//       snd_hda:     33 []


        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 502); // 0x022f0005
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 503); // 0x022f0009
//       snd_hda:     34 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 0 4
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 504); // 0x022f000f
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 505); // 0x022f0012
//       snd_hda:     amp capabilities 34 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 506); // 0x022f000d
//       snd_hda:     amp capabilities 34 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00400281, 507); // 0x022f0009
//       snd_hda:     34 AC_WID_PIN ['AC_WCAP_STEREO', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 0 0 4

        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_CONFIG_DEFAULT, 0x00000000, 0x400000f0, 508); // 0x022f1c00
//       snd_hda:     pin config default 0x400000f0 port conn 1 location 0x0 loc ext 0 loc geom 0 default device 0 conn type 0 color 0 misc 0 def assoc 15 seq 0
//       snd_hda:     pin config default 0x400000f0 port conn NO CONN loc ext Ext loc geom N/A default device Line Out conn type Unknown color Unknown misc undef def assoc 15 seq 0

        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_PARAMETERS, 0x0000000c, 0x00000024, 509); // 0x022f000c
//       snd_hda:     34 ['AC_PINCAP_IN', 'AC_PINCAP_PRES_DETECT'] []

        retval = snd_hda_codec_read_check(codec, 0x22, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 510); // 0x022f0700
        snd_hda_codec_write(codec, 0x22, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x02270700
//       snd_hda:     34 []


        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 513); // 0x023f0005
        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 514); // 0x023f0009
//       snd_hda:     35 AC_WID_BEEP ['AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DELAY', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 0 0 7
        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 515); // 0x023f000f
        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 516); // 0x023f0012
//       snd_hda:     amp capabilities 35 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 517); // 0x023f000d
//       snd_hda:     amp capabilities 35 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x23, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00700200, 518); // 0x023f0009
//       snd_hda:     35 AC_WID_BEEP ['AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DELAY', 'AC_WCAP_DIGITAL', 'AC_WCAP_TYPE'] 0 0 7


        snd_hda_codec_write(codec, 0x23, 0, AC_VERB_SET_BEEP_CONTROL, 0x00000000); // 0x02370a00

        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 521); // 0x024f0005
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 522); // 0x024f0009
//       snd_hda:     36 AC_WID_VENDOR ['AC_WCAP_STEREO', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_PROC_WID', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 7 0 15
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 523); // 0x024f000f
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 524); // 0x024f0012
//       snd_hda:     amp capabilities 36 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 525); // 0x024f000d
//       snd_hda:     amp capabilities 36 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00f0e2c1, 526); // 0x024f0009
//       snd_hda:     36 AC_WID_VENDOR ['AC_WCAP_STEREO', 'AC_WCAP_TYPE', 'AC_WCAP_UNSOL_CAP', 'AC_WCAP_DELAY', 'AC_WCAP_PROC_WID', 'AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DIGITAL'] 7 0 15
        retval = snd_hda_codec_read_check(codec, 0x24, 0, AC_VERB_PARAMETERS, 0x00000010, 0x00008000, 527); // 0x024f0010

       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (527)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000000 }, //   coef read 527
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000000, 528 ); //   coef read 528

       // read  node 0x24 arg 1 0x0000 arg 2 0x0001 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (531)
 //       { 0, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000 }, //   coef read 531
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000, 532 ); //   coef read 532

       // read  node 0x24 arg 1 0x0000 arg 2 0x0002 arg 3 0x0000 arg 4 0x0000 data 0x0000003a (535)
 //       { 0, CS4208_VENDOR_NID, 0x0002, 0x0000, 0x0000003a }, //   coef read 535
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0002, 0x0000, 0x0000003a, 536 ); //   coef read 536

       // read  node 0x24 arg 1 0x0000 arg 2 0x0003 arg 3 0x0000 arg 4 0x0000 data 0x0000fbaa (539)
 //       { 0, CS4208_VENDOR_NID, 0x0003, 0x0000, 0x0000fbaa }, //   coef read 539
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0003, 0x0000, 0x0000fbaa, 540 ); //   coef read 540

       // read  node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (543)
 //       { 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00009f9f }, //   coef read 543
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00009f9f, 544 ); //   coef read 544

       // read  node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (547)
 //       { 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00009f9f }, //   coef read 547
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00009f9f, 548 ); //   coef read 548

       // read  node 0x24 arg 1 0x0000 arg 2 0x0006 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (551)
 //       { 0, CS4208_VENDOR_NID, 0x0006, 0x0000, 0x00009f9f }, //   coef read 551
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0006, 0x0000, 0x00009f9f, 552 ); //   coef read 552

       // read  node 0x24 arg 1 0x0000 arg 2 0x0007 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (555)
 //       { 0, CS4208_VENDOR_NID, 0x0007, 0x0000, 0x00009f9f }, //   coef read 555
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0007, 0x0000, 0x00009f9f, 556 ); //   coef read 556

       // read  node 0x24 arg 1 0x0000 arg 2 0x0008 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (559)
 //       { 0, CS4208_VENDOR_NID, 0x0008, 0x0000, 0x00001f1f }, //   coef read 559
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0008, 0x0000, 0x00001f1f, 560 ); //   coef read 560

       // read  node 0x24 arg 1 0x0000 arg 2 0x0009 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (563)
 //       { 0, CS4208_VENDOR_NID, 0x0009, 0x0000, 0x00001f1f }, //   coef read 563
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0009, 0x0000, 0x00001f1f, 564 ); //   coef read 564

       // read  node 0x24 arg 1 0x0000 arg 2 0x000a arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (567)
 //       { 0, CS4208_VENDOR_NID, 0x000a, 0x0000, 0x00001f1f }, //   coef read 567
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000a, 0x0000, 0x00001f1f, 568 ); //   coef read 568

       // read  node 0x24 arg 1 0x0000 arg 2 0x000b arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (571)
 //       { 0, CS4208_VENDOR_NID, 0x000b, 0x0000, 0x00001f1f }, //   coef read 571
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000b, 0x0000, 0x00001f1f, 572 ); //   coef read 572

       // read  node 0x24 arg 1 0x0000 arg 2 0x000c arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (575)
 //       { 0, CS4208_VENDOR_NID, 0x000c, 0x0000, 0x00009f9f }, //   coef read 575
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000c, 0x0000, 0x00009f9f, 576 ); //   coef read 576

       // read  node 0x24 arg 1 0x0000 arg 2 0x000d arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (579)
 //       { 0, CS4208_VENDOR_NID, 0x000d, 0x0000, 0x00009f9f }, //   coef read 579
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000d, 0x0000, 0x00009f9f, 580 ); //   coef read 580

       // read  node 0x24 arg 1 0x0000 arg 2 0x000e arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (583)
 //       { 0, CS4208_VENDOR_NID, 0x000e, 0x0000, 0x00009f9f }, //   coef read 583
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000e, 0x0000, 0x00009f9f, 584 ); //   coef read 584

       // read  node 0x24 arg 1 0x0000 arg 2 0x000f arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (587)
 //       { 0, CS4208_VENDOR_NID, 0x000f, 0x0000, 0x00009f9f }, //   coef read 587
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x000f, 0x0000, 0x00009f9f, 588 ); //   coef read 588

       // read  node 0x24 arg 1 0x0000 arg 2 0x0010 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (591)
 //       { 0, CS4208_VENDOR_NID, 0x0010, 0x0000, 0x00001f1f }, //   coef read 591
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0010, 0x0000, 0x00001f1f, 592 ); //   coef read 592

       // read  node 0x24 arg 1 0x0000 arg 2 0x0011 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (595)
 //       { 0, CS4208_VENDOR_NID, 0x0011, 0x0000, 0x00001f1f }, //   coef read 595
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0011, 0x0000, 0x00001f1f, 596 ); //   coef read 596

       // read  node 0x24 arg 1 0x0000 arg 2 0x0012 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (599)
 //       { 0, CS4208_VENDOR_NID, 0x0012, 0x0000, 0x00001f1f }, //   coef read 599
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0012, 0x0000, 0x00001f1f, 600 ); //   coef read 600

       // read  node 0x24 arg 1 0x0000 arg 2 0x0013 arg 3 0x0000 arg 4 0x0000 data 0x00001f1f (603)
 //       { 0, CS4208_VENDOR_NID, 0x0013, 0x0000, 0x00001f1f }, //   coef read 603
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0013, 0x0000, 0x00001f1f, 604 ); //   coef read 604

       // read  node 0x24 arg 1 0x0000 arg 2 0x0014 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (607)
 //       { 0, CS4208_VENDOR_NID, 0x0014, 0x0000, 0x00000000 }, //   coef read 607
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0014, 0x0000, 0x00000000, 608 ); //   coef read 608

       // read  node 0x24 arg 1 0x0000 arg 2 0x0015 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (611)
 //       { 0, CS4208_VENDOR_NID, 0x0015, 0x0000, 0x00000000 }, //   coef read 611
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0015, 0x0000, 0x00000000, 612 ); //   coef read 612

       // read  node 0x24 arg 1 0x0000 arg 2 0x0016 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (615)
 //       { 0, CS4208_VENDOR_NID, 0x0016, 0x0000, 0x00000000 }, //   coef read 615
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0016, 0x0000, 0x00000000, 616 ); //   coef read 616

       // read  node 0x24 arg 1 0x0000 arg 2 0x0017 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (619)
 //       { 0, CS4208_VENDOR_NID, 0x0017, 0x0000, 0x00000000 }, //   coef read 619
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0017, 0x0000, 0x00000000, 620 ); //   coef read 620

       // read  node 0x24 arg 1 0x0000 arg 2 0x0018 arg 3 0x0000 arg 4 0x0000 data 0x00000400 (623)
 //       { 0, CS4208_VENDOR_NID, 0x0018, 0x0000, 0x00000400 }, //   coef read 623
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0018, 0x0000, 0x00000400, 624 ); //   coef read 624

       // read  node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 data 0x00000088 (627)
 //       { 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00000088 }, //   coef read 627
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00000088, 628 ); //   coef read 628

       // read  node 0x24 arg 1 0x0000 arg 2 0x001a arg 3 0x0000 arg 4 0x0000 data 0x000000f3 (631)
 //       { 0, CS4208_VENDOR_NID, 0x001a, 0x0000, 0x000000f3 }, //   coef read 631
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001a, 0x0000, 0x000000f3, 632 ); //   coef read 632

       // read  node 0x24 arg 1 0x0000 arg 2 0x001b arg 3 0x0000 arg 4 0x0000 data 0x00000002 (635)
 //       { 0, CS4208_VENDOR_NID, 0x001b, 0x0000, 0x00000002 }, //   coef read 635
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001b, 0x0000, 0x00000002, 636 ); //   coef read 636

       // read  node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 data 0x00000103 (639)
 //       { 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000103 }, //   coef read 639
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000103, 640 ); //   coef read 640

       // read  node 0x24 arg 1 0x0000 arg 2 0x001d arg 3 0x0000 arg 4 0x0000 data 0x00000bdf (643)
 //       { 0, CS4208_VENDOR_NID, 0x001d, 0x0000, 0x00000bdf }, //   coef read 643
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001d, 0x0000, 0x00000bdf, 644 ); //   coef read 644

       // read  node 0x24 arg 1 0x0000 arg 2 0x001e arg 3 0x0000 arg 4 0x0000 data 0x0000016d (647)
 //       { 0, CS4208_VENDOR_NID, 0x001e, 0x0000, 0x0000016d }, //   coef read 647
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001e, 0x0000, 0x0000016d, 648 ); //   coef read 648

       // read  node 0x24 arg 1 0x0000 arg 2 0x001f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (651)
 //       { 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000 }, //   coef read 651
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x001f, 0x0000, 0x00000000, 652 ); //   coef read 652

       // read  node 0x24 arg 1 0x0000 arg 2 0x0020 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (655)
 //       { 0, CS4208_VENDOR_NID, 0x0020, 0x0000, 0x00000000 }, //   coef read 655
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0020, 0x0000, 0x00000000, 656 ); //   coef read 656

       // read  node 0x24 arg 1 0x0000 arg 2 0x0021 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (659)
 //       { 0, CS4208_VENDOR_NID, 0x0021, 0x0000, 0x00000000 }, //   coef read 659
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0021, 0x0000, 0x00000000, 660 ); //   coef read 660

       // read  node 0x24 arg 1 0x0000 arg 2 0x0022 arg 3 0x0000 arg 4 0x0000 data 0x00000080 (663)
 //       { 0, CS4208_VENDOR_NID, 0x0022, 0x0000, 0x00000080 }, //   coef read 663
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0022, 0x0000, 0x00000080, 664 ); //   coef read 664

       // read  node 0x24 arg 1 0x0000 arg 2 0x0023 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (667)
 //       { 0, CS4208_VENDOR_NID, 0x0023, 0x0000, 0x00000000 }, //   coef read 667
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0023, 0x0000, 0x00000000, 668 ); //   coef read 668

       // read  node 0x24 arg 1 0x0000 arg 2 0x0024 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (671)
 //       { 0, CS4208_VENDOR_NID, 0x0024, 0x0000, 0x00000000 }, //   coef read 671
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0024, 0x0000, 0x00000000, 672 ); //   coef read 672

       // read  node 0x24 arg 1 0x0000 arg 2 0x0025 arg 3 0x0000 arg 4 0x0000 data 0x00000001 (675)
 //       { 0, CS4208_VENDOR_NID, 0x0025, 0x0000, 0x00000001 }, //   coef read 675
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0025, 0x0000, 0x00000001, 676 ); //   coef read 676

       // read  node 0x24 arg 1 0x0000 arg 2 0x0026 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (679)
 //       { 0, CS4208_VENDOR_NID, 0x0026, 0x0000, 0x00000000 }, //   coef read 679
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0026, 0x0000, 0x00000000, 680 ); //   coef read 680

       // read  node 0x24 arg 1 0x0000 arg 2 0x0027 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (683)
 //       { 0, CS4208_VENDOR_NID, 0x0027, 0x0000, 0x00000000 }, //   coef read 683
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0027, 0x0000, 0x00000000, 684 ); //   coef read 684

       // read  node 0x24 arg 1 0x0000 arg 2 0x0028 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (687)
 //       { 0, CS4208_VENDOR_NID, 0x0028, 0x0000, 0x00000000 }, //   coef read 687
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0028, 0x0000, 0x00000000, 688 ); //   coef read 688

       // read  node 0x24 arg 1 0x0000 arg 2 0x0029 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (691)
 //       { 0, CS4208_VENDOR_NID, 0x0029, 0x0000, 0x00000000 }, //   coef read 691
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0029, 0x0000, 0x00000000, 692 ); //   coef read 692

       // read  node 0x24 arg 1 0x0000 arg 2 0x002a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (695)
 //       { 0, CS4208_VENDOR_NID, 0x002a, 0x0000, 0x00000000 }, //   coef read 695
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002a, 0x0000, 0x00000000, 696 ); //   coef read 696

       // read  node 0x24 arg 1 0x0000 arg 2 0x002b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (699)
 //       { 0, CS4208_VENDOR_NID, 0x002b, 0x0000, 0x00000000 }, //   coef read 699
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002b, 0x0000, 0x00000000, 700 ); //   coef read 700

       // read  node 0x24 arg 1 0x0000 arg 2 0x002c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (703)
 //       { 0, CS4208_VENDOR_NID, 0x002c, 0x0000, 0x00000000 }, //   coef read 703
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002c, 0x0000, 0x00000000, 704 ); //   coef read 704

       // read  node 0x24 arg 1 0x0000 arg 2 0x002d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (707)
 //       { 0, CS4208_VENDOR_NID, 0x002d, 0x0000, 0x00000000 }, //   coef read 707
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002d, 0x0000, 0x00000000, 708 ); //   coef read 708

       // read  node 0x24 arg 1 0x0000 arg 2 0x002e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (711)
 //       { 0, CS4208_VENDOR_NID, 0x002e, 0x0000, 0x00000000 }, //   coef read 711
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002e, 0x0000, 0x00000000, 712 ); //   coef read 712

       // read  node 0x24 arg 1 0x0000 arg 2 0x002f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (715)
 //       { 0, CS4208_VENDOR_NID, 0x002f, 0x0000, 0x00000000 }, //   coef read 715
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x002f, 0x0000, 0x00000000, 716 ); //   coef read 716

       // read  node 0x24 arg 1 0x0000 arg 2 0x0030 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (719)
 //       { 0, CS4208_VENDOR_NID, 0x0030, 0x0000, 0x00000000 }, //   coef read 719
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0030, 0x0000, 0x00000000, 720 ); //   coef read 720

       // read  node 0x24 arg 1 0x0000 arg 2 0x0031 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (723)
 //       { 0, CS4208_VENDOR_NID, 0x0031, 0x0000, 0x00000000 }, //   coef read 723
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0031, 0x0000, 0x00000000, 724 ); //   coef read 724

       // read  node 0x24 arg 1 0x0000 arg 2 0x0032 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (727)
 //       { 0, CS4208_VENDOR_NID, 0x0032, 0x0000, 0x00000000 }, //   coef read 727
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0032, 0x0000, 0x00000000, 728 ); //   coef read 728

       // read  node 0x24 arg 1 0x0000 arg 2 0x0033 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (731)
 //       { 0, CS4208_VENDOR_NID, 0x0033, 0x0000, 0x00000000 }, //   coef read 731
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0033, 0x0000, 0x00000000, 732 ); //   coef read 732

       // read  node 0x24 arg 1 0x0000 arg 2 0x0034 arg 3 0x0000 arg 4 0x0000 data 0x00001b00 (735)
 //       { 0, CS4208_VENDOR_NID, 0x0034, 0x0000, 0x00001b00 }, //   coef read 735
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0034, 0x0000, 0x00001b00, 736 ); //   coef read 736

       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (739)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 739
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 740 ); //   coef read 740

       // read  node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0000 arg 4 0x0000 data 0x00000014 (743)
 //       { 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000014 }, //   coef read 743
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000014, 744 ); //   coef read 744

       // read  node 0x24 arg 1 0x0000 arg 2 0x0037 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (747)
 //       { 0, CS4208_VENDOR_NID, 0x0037, 0x0000, 0x00000000 }, //   coef read 747
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0037, 0x0000, 0x00000000, 748 ); //   coef read 748

       // read  node 0x24 arg 1 0x0000 arg 2 0x0038 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (751)
 //       { 0, CS4208_VENDOR_NID, 0x0038, 0x0000, 0x00000000 }, //   coef read 751
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0038, 0x0000, 0x00000000, 752 ); //   coef read 752

       // read  node 0x24 arg 1 0x0000 arg 2 0x0039 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (755)
 //       { 0, CS4208_VENDOR_NID, 0x0039, 0x0000, 0x00000000 }, //   coef read 755
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0039, 0x0000, 0x00000000, 756 ); //   coef read 756

       // read  node 0x24 arg 1 0x0000 arg 2 0x003a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (759)
 //       { 0, CS4208_VENDOR_NID, 0x003a, 0x0000, 0x00000000 }, //   coef read 759
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003a, 0x0000, 0x00000000, 760 ); //   coef read 760

       // read  node 0x24 arg 1 0x0000 arg 2 0x003b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (763)
 //       { 0, CS4208_VENDOR_NID, 0x003b, 0x0000, 0x00000000 }, //   coef read 763
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003b, 0x0000, 0x00000000, 764 ); //   coef read 764

       // read  node 0x24 arg 1 0x0000 arg 2 0x003c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (767)
 //       { 0, CS4208_VENDOR_NID, 0x003c, 0x0000, 0x00000000 }, //   coef read 767
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003c, 0x0000, 0x00000000, 768 ); //   coef read 768

       // read  node 0x24 arg 1 0x0000 arg 2 0x003d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (771)
 //       { 0, CS4208_VENDOR_NID, 0x003d, 0x0000, 0x00000000 }, //   coef read 771
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003d, 0x0000, 0x00000000, 772 ); //   coef read 772

       // read  node 0x24 arg 1 0x0000 arg 2 0x003e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (775)
 //       { 0, CS4208_VENDOR_NID, 0x003e, 0x0000, 0x00000000 }, //   coef read 775
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003e, 0x0000, 0x00000000, 776 ); //   coef read 776

       // read  node 0x24 arg 1 0x0000 arg 2 0x003f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (779)
 //       { 0, CS4208_VENDOR_NID, 0x003f, 0x0000, 0x00000000 }, //   coef read 779
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x003f, 0x0000, 0x00000000, 780 ); //   coef read 780

       // read  node 0x24 arg 1 0x0000 arg 2 0x0040 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (783)
 //       { 0, CS4208_VENDOR_NID, 0x0040, 0x0000, 0x00000000 }, //   coef read 783
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0040, 0x0000, 0x00000000, 784 ); //   coef read 784

       // read  node 0x24 arg 1 0x0000 arg 2 0x0041 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (787)
 //       { 0, CS4208_VENDOR_NID, 0x0041, 0x0000, 0x00000000 }, //   coef read 787
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0041, 0x0000, 0x00000000, 788 ); //   coef read 788

       // read  node 0x24 arg 1 0x0000 arg 2 0x0042 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (791)
 //       { 0, CS4208_VENDOR_NID, 0x0042, 0x0000, 0x00000000 }, //   coef read 791
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0042, 0x0000, 0x00000000, 792 ); //   coef read 792

       // read  node 0x24 arg 1 0x0000 arg 2 0x0043 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (795)
 //       { 0, CS4208_VENDOR_NID, 0x0043, 0x0000, 0x00000000 }, //   coef read 795
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0043, 0x0000, 0x00000000, 796 ); //   coef read 796

       // read  node 0x24 arg 1 0x0000 arg 2 0x0044 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (799)
 //       { 0, CS4208_VENDOR_NID, 0x0044, 0x0000, 0x00000000 }, //   coef read 799
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0044, 0x0000, 0x00000000, 800 ); //   coef read 800

       // read  node 0x24 arg 1 0x0000 arg 2 0x0045 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (803)
 //       { 0, CS4208_VENDOR_NID, 0x0045, 0x0000, 0x00000000 }, //   coef read 803
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0045, 0x0000, 0x00000000, 804 ); //   coef read 804

       // read  node 0x24 arg 1 0x0000 arg 2 0x0046 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (807)
 //       { 0, CS4208_VENDOR_NID, 0x0046, 0x0000, 0x00000000 }, //   coef read 807
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0046, 0x0000, 0x00000000, 808 ); //   coef read 808

       // read  node 0x24 arg 1 0x0000 arg 2 0x0047 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (811)
 //       { 0, CS4208_VENDOR_NID, 0x0047, 0x0000, 0x00000000 }, //   coef read 811
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0047, 0x0000, 0x00000000, 812 ); //   coef read 812

       // read  node 0x24 arg 1 0x0000 arg 2 0x0048 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (815)
 //       { 0, CS4208_VENDOR_NID, 0x0048, 0x0000, 0x00000000 }, //   coef read 815
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0048, 0x0000, 0x00000000, 816 ); //   coef read 816

       // read  node 0x24 arg 1 0x0000 arg 2 0x0049 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (819)
 //       { 0, CS4208_VENDOR_NID, 0x0049, 0x0000, 0x00000000 }, //   coef read 819
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0049, 0x0000, 0x00000000, 820 ); //   coef read 820

       // read  node 0x24 arg 1 0x0000 arg 2 0x004a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (823)
 //       { 0, CS4208_VENDOR_NID, 0x004a, 0x0000, 0x00000000 }, //   coef read 823
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004a, 0x0000, 0x00000000, 824 ); //   coef read 824

       // read  node 0x24 arg 1 0x0000 arg 2 0x004b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (827)
 //       { 0, CS4208_VENDOR_NID, 0x004b, 0x0000, 0x00000000 }, //   coef read 827
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004b, 0x0000, 0x00000000, 828 ); //   coef read 828

       // read  node 0x24 arg 1 0x0000 arg 2 0x004c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (831)
 //       { 0, CS4208_VENDOR_NID, 0x004c, 0x0000, 0x00000000 }, //   coef read 831
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004c, 0x0000, 0x00000000, 832 ); //   coef read 832

       // read  node 0x24 arg 1 0x0000 arg 2 0x004d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (835)
 //       { 0, CS4208_VENDOR_NID, 0x004d, 0x0000, 0x00000000 }, //   coef read 835
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004d, 0x0000, 0x00000000, 836 ); //   coef read 836

       // read  node 0x24 arg 1 0x0000 arg 2 0x004e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (839)
 //       { 0, CS4208_VENDOR_NID, 0x004e, 0x0000, 0x00000000 }, //   coef read 839
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004e, 0x0000, 0x00000000, 840 ); //   coef read 840

       // read  node 0x24 arg 1 0x0000 arg 2 0x004f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (843)
 //       { 0, CS4208_VENDOR_NID, 0x004f, 0x0000, 0x00000000 }, //   coef read 843
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x004f, 0x0000, 0x00000000, 844 ); //   coef read 844

       // read  node 0x24 arg 1 0x0000 arg 2 0x0050 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (847)
 //       { 0, CS4208_VENDOR_NID, 0x0050, 0x0000, 0x00000000 }, //   coef read 847
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0050, 0x0000, 0x00000000, 848 ); //   coef read 848

       // read  node 0x24 arg 1 0x0000 arg 2 0x0051 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (851)
 //       { 0, CS4208_VENDOR_NID, 0x0051, 0x0000, 0x00000000 }, //   coef read 851
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0051, 0x0000, 0x00000000, 852 ); //   coef read 852

       // read  node 0x24 arg 1 0x0000 arg 2 0x0052 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (855)
 //       { 0, CS4208_VENDOR_NID, 0x0052, 0x0000, 0x00000000 }, //   coef read 855
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0052, 0x0000, 0x00000000, 856 ); //   coef read 856

       // read  node 0x24 arg 1 0x0000 arg 2 0x0053 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (859)
 //       { 0, CS4208_VENDOR_NID, 0x0053, 0x0000, 0x00000000 }, //   coef read 859
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0053, 0x0000, 0x00000000, 860 ); //   coef read 860

       // read  node 0x24 arg 1 0x0000 arg 2 0x0054 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (863)
 //       { 0, CS4208_VENDOR_NID, 0x0054, 0x0000, 0x00000000 }, //   coef read 863
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0054, 0x0000, 0x00000000, 864 ); //   coef read 864

       // read  node 0x24 arg 1 0x0000 arg 2 0x0055 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (867)
 //       { 0, CS4208_VENDOR_NID, 0x0055, 0x0000, 0x00000000 }, //   coef read 867
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0055, 0x0000, 0x00000000, 868 ); //   coef read 868

       // read  node 0x24 arg 1 0x0000 arg 2 0x0056 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (871)
 //       { 0, CS4208_VENDOR_NID, 0x0056, 0x0000, 0x00000000 }, //   coef read 871
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0056, 0x0000, 0x00000000, 872 ); //   coef read 872

       // read  node 0x24 arg 1 0x0000 arg 2 0x0057 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (875)
 //       { 0, CS4208_VENDOR_NID, 0x0057, 0x0000, 0x00000000 }, //   coef read 875
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0057, 0x0000, 0x00000000, 876 ); //   coef read 876

       // read  node 0x24 arg 1 0x0000 arg 2 0x0058 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (879)
 //       { 0, CS4208_VENDOR_NID, 0x0058, 0x0000, 0x00000000 }, //   coef read 879
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0058, 0x0000, 0x00000000, 880 ); //   coef read 880

       // read  node 0x24 arg 1 0x0000 arg 2 0x0059 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (883)
 //       { 0, CS4208_VENDOR_NID, 0x0059, 0x0000, 0x00000000 }, //   coef read 883
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0059, 0x0000, 0x00000000, 884 ); //   coef read 884

       // read  node 0x24 arg 1 0x0000 arg 2 0x005a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (887)
 //       { 0, CS4208_VENDOR_NID, 0x005a, 0x0000, 0x00000000 }, //   coef read 887
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005a, 0x0000, 0x00000000, 888 ); //   coef read 888

       // read  node 0x24 arg 1 0x0000 arg 2 0x005b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (891)
 //       { 0, CS4208_VENDOR_NID, 0x005b, 0x0000, 0x00000000 }, //   coef read 891
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005b, 0x0000, 0x00000000, 892 ); //   coef read 892

       // read  node 0x24 arg 1 0x0000 arg 2 0x005c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (895)
 //       { 0, CS4208_VENDOR_NID, 0x005c, 0x0000, 0x00000000 }, //   coef read 895
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005c, 0x0000, 0x00000000, 896 ); //   coef read 896

       // read  node 0x24 arg 1 0x0000 arg 2 0x005d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (899)
 //       { 0, CS4208_VENDOR_NID, 0x005d, 0x0000, 0x00000000 }, //   coef read 899
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005d, 0x0000, 0x00000000, 900 ); //   coef read 900

       // read  node 0x24 arg 1 0x0000 arg 2 0x005e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (903)
 //       { 0, CS4208_VENDOR_NID, 0x005e, 0x0000, 0x00000000 }, //   coef read 903
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005e, 0x0000, 0x00000000, 904 ); //   coef read 904

       // read  node 0x24 arg 1 0x0000 arg 2 0x005f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (907)
 //       { 0, CS4208_VENDOR_NID, 0x005f, 0x0000, 0x00000000 }, //   coef read 907
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x005f, 0x0000, 0x00000000, 908 ); //   coef read 908

       // read  node 0x24 arg 1 0x0000 arg 2 0x0060 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (911)
 //       { 0, CS4208_VENDOR_NID, 0x0060, 0x0000, 0x00000000 }, //   coef read 911
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0060, 0x0000, 0x00000000, 912 ); //   coef read 912

       // read  node 0x24 arg 1 0x0000 arg 2 0x0061 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (915)
 //       { 0, CS4208_VENDOR_NID, 0x0061, 0x0000, 0x00000000 }, //   coef read 915
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0061, 0x0000, 0x00000000, 916 ); //   coef read 916

       // read  node 0x24 arg 1 0x0000 arg 2 0x0062 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (919)
 //       { 0, CS4208_VENDOR_NID, 0x0062, 0x0000, 0x00000000 }, //   coef read 919
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0062, 0x0000, 0x00000000, 920 ); //   coef read 920

       // read  node 0x24 arg 1 0x0000 arg 2 0x0063 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (923)
 //       { 0, CS4208_VENDOR_NID, 0x0063, 0x0000, 0x00000000 }, //   coef read 923
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0063, 0x0000, 0x00000000, 924 ); //   coef read 924

       // read  node 0x24 arg 1 0x0000 arg 2 0x0064 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (927)
 //       { 0, CS4208_VENDOR_NID, 0x0064, 0x0000, 0x00000000 }, //   coef read 927
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0064, 0x0000, 0x00000000, 928 ); //   coef read 928

       // read  node 0x24 arg 1 0x0000 arg 2 0x0065 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (931)
 //       { 0, CS4208_VENDOR_NID, 0x0065, 0x0000, 0x00000000 }, //   coef read 931
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0065, 0x0000, 0x00000000, 932 ); //   coef read 932

       // read  node 0x24 arg 1 0x0000 arg 2 0x0066 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (935)
 //       { 0, CS4208_VENDOR_NID, 0x0066, 0x0000, 0x00000000 }, //   coef read 935
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0066, 0x0000, 0x00000000, 936 ); //   coef read 936

       // read  node 0x24 arg 1 0x0000 arg 2 0x0067 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (939)
 //       { 0, CS4208_VENDOR_NID, 0x0067, 0x0000, 0x00000000 }, //   coef read 939
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0067, 0x0000, 0x00000000, 940 ); //   coef read 940

       // read  node 0x24 arg 1 0x0000 arg 2 0x0068 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (943)
 //       { 0, CS4208_VENDOR_NID, 0x0068, 0x0000, 0x00000000 }, //   coef read 943
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0068, 0x0000, 0x00000000, 944 ); //   coef read 944

       // read  node 0x24 arg 1 0x0000 arg 2 0x0069 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (947)
 //       { 0, CS4208_VENDOR_NID, 0x0069, 0x0000, 0x00000000 }, //   coef read 947
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0069, 0x0000, 0x00000000, 948 ); //   coef read 948

       // read  node 0x24 arg 1 0x0000 arg 2 0x006a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (951)
 //       { 0, CS4208_VENDOR_NID, 0x006a, 0x0000, 0x00000000 }, //   coef read 951
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006a, 0x0000, 0x00000000, 952 ); //   coef read 952

       // read  node 0x24 arg 1 0x0000 arg 2 0x006b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (955)
 //       { 0, CS4208_VENDOR_NID, 0x006b, 0x0000, 0x00000000 }, //   coef read 955
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006b, 0x0000, 0x00000000, 956 ); //   coef read 956

       // read  node 0x24 arg 1 0x0000 arg 2 0x006c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (959)
 //       { 0, CS4208_VENDOR_NID, 0x006c, 0x0000, 0x00000000 }, //   coef read 959
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006c, 0x0000, 0x00000000, 960 ); //   coef read 960

       // read  node 0x24 arg 1 0x0000 arg 2 0x006d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (963)
 //       { 0, CS4208_VENDOR_NID, 0x006d, 0x0000, 0x00000000 }, //   coef read 963
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006d, 0x0000, 0x00000000, 964 ); //   coef read 964

       // read  node 0x24 arg 1 0x0000 arg 2 0x006e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (967)
 //       { 0, CS4208_VENDOR_NID, 0x006e, 0x0000, 0x00000000 }, //   coef read 967
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006e, 0x0000, 0x00000000, 968 ); //   coef read 968

       // read  node 0x24 arg 1 0x0000 arg 2 0x006f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (971)
 //       { 0, CS4208_VENDOR_NID, 0x006f, 0x0000, 0x00000000 }, //   coef read 971
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x006f, 0x0000, 0x00000000, 972 ); //   coef read 972

       // read  node 0x24 arg 1 0x0000 arg 2 0x0070 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (975)
 //       { 0, CS4208_VENDOR_NID, 0x0070, 0x0000, 0x00000000 }, //   coef read 975
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0070, 0x0000, 0x00000000, 976 ); //   coef read 976

       // read  node 0x24 arg 1 0x0000 arg 2 0x0071 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (979)
 //       { 0, CS4208_VENDOR_NID, 0x0071, 0x0000, 0x00000000 }, //   coef read 979
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0071, 0x0000, 0x00000000, 980 ); //   coef read 980

       // read  node 0x24 arg 1 0x0000 arg 2 0x0072 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (983)
 //       { 0, CS4208_VENDOR_NID, 0x0072, 0x0000, 0x00000000 }, //   coef read 983
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0072, 0x0000, 0x00000000, 984 ); //   coef read 984

       // read  node 0x24 arg 1 0x0000 arg 2 0x0073 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (987)
 //       { 0, CS4208_VENDOR_NID, 0x0073, 0x0000, 0x00000000 }, //   coef read 987
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0073, 0x0000, 0x00000000, 988 ); //   coef read 988

       // read  node 0x24 arg 1 0x0000 arg 2 0x0074 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (991)
 //       { 0, CS4208_VENDOR_NID, 0x0074, 0x0000, 0x00000000 }, //   coef read 991
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0074, 0x0000, 0x00000000, 992 ); //   coef read 992

       // read  node 0x24 arg 1 0x0000 arg 2 0x0075 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (995)
 //       { 0, CS4208_VENDOR_NID, 0x0075, 0x0000, 0x00000000 }, //   coef read 995
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0075, 0x0000, 0x00000000, 996 ); //   coef read 996

       // read  node 0x24 arg 1 0x0000 arg 2 0x0076 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (999)
 //       { 0, CS4208_VENDOR_NID, 0x0076, 0x0000, 0x00000000 }, //   coef read 999
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0076, 0x0000, 0x00000000, 1000 ); //   coef read 1000

       // read  node 0x24 arg 1 0x0000 arg 2 0x0077 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1003)
 //       { 0, CS4208_VENDOR_NID, 0x0077, 0x0000, 0x00000000 }, //   coef read 1003
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0077, 0x0000, 0x00000000, 1004 ); //   coef read 1004

       // read  node 0x24 arg 1 0x0000 arg 2 0x0078 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1007)
 //       { 0, CS4208_VENDOR_NID, 0x0078, 0x0000, 0x00000000 }, //   coef read 1007
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0078, 0x0000, 0x00000000, 1008 ); //   coef read 1008

       // read  node 0x24 arg 1 0x0000 arg 2 0x0079 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1011)
 //       { 0, CS4208_VENDOR_NID, 0x0079, 0x0000, 0x00000000 }, //   coef read 1011
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0079, 0x0000, 0x00000000, 1012 ); //   coef read 1012

       // read  node 0x24 arg 1 0x0000 arg 2 0x007a arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1015)
 //       { 0, CS4208_VENDOR_NID, 0x007a, 0x0000, 0x00000000 }, //   coef read 1015
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007a, 0x0000, 0x00000000, 1016 ); //   coef read 1016

       // read  node 0x24 arg 1 0x0000 arg 2 0x007b arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1019)
 //       { 0, CS4208_VENDOR_NID, 0x007b, 0x0000, 0x00000000 }, //   coef read 1019
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007b, 0x0000, 0x00000000, 1020 ); //   coef read 1020

       // read  node 0x24 arg 1 0x0000 arg 2 0x007c arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1023)
 //       { 0, CS4208_VENDOR_NID, 0x007c, 0x0000, 0x00000000 }, //   coef read 1023
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007c, 0x0000, 0x00000000, 1024 ); //   coef read 1024

       // read  node 0x24 arg 1 0x0000 arg 2 0x007d arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1027)
 //       { 0, CS4208_VENDOR_NID, 0x007d, 0x0000, 0x00000000 }, //   coef read 1027
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007d, 0x0000, 0x00000000, 1028 ); //   coef read 1028

       // read  node 0x24 arg 1 0x0000 arg 2 0x007e arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1031)
 //       { 0, CS4208_VENDOR_NID, 0x007e, 0x0000, 0x00000000 }, //   coef read 1031
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007e, 0x0000, 0x00000000, 1032 ); //   coef read 1032


       // read  node 0x24 arg 1 0x0000 arg 2 0x007f arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1035)
 //       { 0, CS4208_VENDOR_NID, 0x007f, 0x0000, 0x00000000 }, //   coef read 1035
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x007f, 0x0000, 0x00000000, 1036 ); //   coef read 1036


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // write node 0x24 arg 1 0x0000 arg 2 0x0040 arg 3 0x9999 arg 4 0x0000 (1041)
 //       { 1, CS4208_VENDOR_NID, 0x0040, 0x9999, 0x00000000 }, //   coef write 1041
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0040, 0x9999, 0x00000000, 1042 ); //   coef write 1042

       // write node 0x24 arg 1 0x0000 arg 2 0x0050 arg 3 0x008b arg 4 0x0000 (1045)
 //       { 1, CS4208_VENDOR_NID, 0x0050, 0x008b, 0x00000000 }, //   coef write 1045
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0050, 0x008b, 0x00000000, 1046 ); //   coef write 1046


       // write node 0x24 arg 1 0x0000 arg 2 0x0040 arg 3 0x0000 arg 4 0x0000 (1049)
 //       { 1, CS4208_VENDOR_NID, 0x0040, 0x0000, 0x00000000 }, //   coef write 1049
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0040, 0x0000, 0x00000000, 1050 ); //   coef write 1050

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

       // write node 0x24 arg 1 0x0000 arg 2 0x0022 arg 3 0x0080 arg 4 0x0000 (1058)
 //       { 1, CS4208_VENDOR_NID, 0x0022, 0x0080, 0x00000000 }, //   coef write 1058
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0022, 0x0080, 0x00000000, 1059 ); //   coef write 1059


       // write node 0x24 arg 1 0x0000 arg 2 0x002a arg 3 0x0000 arg 4 0x0000 (1062)
 //       { 1, CS4208_VENDOR_NID, 0x002a, 0x0000, 0x00000000 }, //   coef write 1062
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x002a, 0x0000, 0x00000000, 1063 ); //   coef write 1063

        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000005, 0x00000000, 1067); // 0x025f0005
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1068); // 0x025f0009
//       snd_hda:     37 AC_WID_AUD_OUT ['AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DELAY', 'AC_WCAP_TYPE'] 0 0 0
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000f, 0x00000000, 1069); // 0x025f000f
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000a, 0x00000000, 1070); // 0x025f000a
//       snd_hda:     pcm params           37 bits:  rates: 
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000b, 0x00000000, 1071); // 0x025f000b
//       snd_hda:     stream format params 37 
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000012, 0x00000000, 1072); // 0x025f0012
//       snd_hda:     amp capabilities 37 output 0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x0000000d, 0x00000000, 1073); // 0x025f000d
//       snd_hda:     amp capabilities 37 input  0x00000000 offset 0x00 numsteps 0x00 stepsize 0x00 mute 0
        retval = snd_hda_codec_read_check(codec, 0x25, 0, AC_VERB_PARAMETERS, 0x00000009, 0x00000000, 1074); // 0x025f0009
//       snd_hda:     37 AC_WID_AUD_OUT ['AC_WCAP_CHAN_CNT_EXT', 'AC_WCAP_DELAY', 'AC_WCAP_TYPE'] 0 0 0



       // read  node 0x24 arg 1 0x0000 arg 2 0x0003 arg 3 0x0000 arg 4 0x0000 data 0x0000fbaa (1075)
 //       { 0, CS4208_VENDOR_NID, 0x0003, 0x0000, 0x0000fbaa }, //   coef read 1075
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0003, 0x0000, 0x0000fbaa, 1076 ); //   coef read 1076

       // write node 0x24 arg 1 0x0000 arg 2 0x0003 arg 3 0x0baa arg 4 0x0000 (1079)
 //       { 1, CS4208_VENDOR_NID, 0x0003, 0x0baa, 0x00000000 }, //   coef write 1079
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0003, 0x0baa, 0x00000000, 1080 ); //   coef write 1080

       // read  node 0x24 arg 1 0x0000 arg 2 0x0001 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1083)
 //       { 0, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000 }, //   coef read 1083
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000, 1084 ); //   coef read 1084

       // write node 0x24 arg 1 0x0000 arg 2 0x0001 arg 3 0x0000 arg 4 0x0000 (1087)
 //       { 1, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000 }, //   coef write 1087
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0001, 0x0000, 0x00000000, 1088 ); //   coef write 1088

       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1091)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000000 }, //   coef read 1091
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000000, 1092 ); //   coef read 1092

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0004 arg 4 0x0000 (1095)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x0004, 0x00000000 }, //   coef write 1095
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x0004, 0x00000000, 1096 ); //   coef write 1096


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000004); // 0x00171604
//       snd_hda:     gpio enable 1 0x04

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000004, 1102); // 0x001f1500
//       snd_hda:     gpio data 1 0x04


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000006); // 0x00171606
//       snd_hda:     gpio enable 1 0x06

        retval = snd_hda_codec_read_check(codec, codec->core.afg, 0, AC_VERB_GET_GPIO_DATA, 0x00000000, 0x00000006, 1105); // 0x001f1500
//       snd_hda:     gpio data 1 0x06

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x00000004 (1119)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000004 }, //   coef read 1119
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000004, 1120 ); //   coef read 1120

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0004 arg 4 0x0000 (1123)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x0004, 0x00000000 }, //   coef write 1123
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x0004, 0x00000000, 1124 ); //   coef write 1124

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // write node 0x24 arg 1 0x0000 arg 2 0x001c arg 3 0x0000 arg 4 0x0000 (1140)
 //       { 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000 }, //   coef write 1140
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001c, 0x0000, 0x00000000, 1141 ); //   coef write 1141


       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 (1144)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00000000 }, //   coef write 1144
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00000000, 1145 ); //   coef write 1145

        snd_hda_codec_write(codec, 0x10, 0, AC_VERB_SET_UNSOLICITED_ENABLE, 0x00000083); // 0x01070883

       // write node 0x24 arg 1 0x0000 arg 2 0x0033 arg 3 0x0001 arg 4 0x0000 (1149)
 //       { 1, CS4208_VENDOR_NID, 0x0033, 0x0001, 0x00000000 }, //   coef write 1149
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0033, 0x0001, 0x00000000, 1150 ); //   coef write 1150


       // write node 0x24 arg 1 0x0000 arg 2 0x0034 arg 3 0x1c01 arg 4 0x0000 (1153)
 //       { 1, CS4208_VENDOR_NID, 0x0034, 0x1c01, 0x00000000 }, //   coef write 1153
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0034, 0x1c01, 0x00000000, 1154 ); //   coef write 1154


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000020); // 0x00171720
//       snd_hda:     gpio direction 1 0x20 in in out in in in in in


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000026); // 0x00171626
//       snd_hda:     gpio enable 1 0x26


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000030); // 0x00171730
//       snd_hda:     gpio direction 1 0x30 in in out out in in in in


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000036); // 0x00171636
//       snd_hda:     gpio enable 1 0x36

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
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

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



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1249)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 1249
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 1250 ); //   coef read 1250

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (1253)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 1253
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 1254 ); //   coef write 1254

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
//       snd_hda:     stream format 6 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1259); // 0x006f0500

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00670500

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000030, 1262); // 0x006f0500

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00670600
//       snd_hda:     conv stream channel map 6 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00670503

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1265); // 0x006f0500

       // read  node 0x24 arg 1 0x0000 arg 2 0x0002 arg 3 0x0000 arg 4 0x0000 data 0x0000003a (1266)
 //       { 0, CS4208_VENDOR_NID, 0x0002, 0x0000, 0x0000003a }, 
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0002, 0x0000, 0x0000003a, 1267 ); 
       // write node 0x24 arg 1 0x0000 arg 2 0x0002 arg 3 0x003a arg 4 0x0000 (1270)
 //       { 1, CS4208_VENDOR_NID, 0x0002, 0x003a, 0x00000000 }, 
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0002, 0x003a, 0x00000000, 1271 ); 
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_CONNECT_SEL, 0x00000002); // 0x00670102


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1276); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00b3 mute 1 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
//       snd_hda:     amp gain/mute 6 0x60a7 mute 1 gain 0x27 39 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1278); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00b3 mute 1 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
//       snd_hda:     amp gain/mute 6 0x50a7 mute 1 gain 0x27 39 index 0x00 left 0 right 1 output 0 input 1  right  input

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1280); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00a7 mute 1 gain 0x27 39
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060a7); // 0x006360a7
//       snd_hda:     amp gain/mute 6 0x60a7 mute 1 gain 0x27 39 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1282); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00a7 mute 1 gain 0x27 39
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050a7); // 0x006350a7
//       snd_hda:     amp gain/mute 6 0x50a7 mute 1 gain 0x27 39 index 0x00 left 0 right 1 output 0 input 1  right  input

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1284); // 0x019b2000
//       snd_hda:     amp gain/mute 25 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 25 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
//       snd_hda:     amp gain/mute 25 0x6000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1286); // 0x019b0000
//       snd_hda:     amp gain/mute 25 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 25 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
//       snd_hda:     amp gain/mute 25 0x5000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 0 input 1  right  input

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000000, 1288); // 0x019b2000
//       snd_hda:     amp gain/mute 25 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 25 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006000); // 0x01936000
//       snd_hda:     amp gain/mute 25 0x6000 mute 0 gain 0x0 0 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000000, 1290); // 0x019b0000
//       snd_hda:     amp gain/mute 25 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 25 0x0000 mute 0 gain 0x0 0
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005000); // 0x01935000
//       snd_hda:     amp gain/mute 25 0x5000 mute 0 gain 0x0 0 index 0x00 left 0 right 1 output 0 input 1  right  input

        retval = snd_hda_codec_read_check(codec, 0x19, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1292); // 0x019f0700
        snd_hda_codec_write(codec, 0x19, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000020); // 0x01970720
//       snd_hda:     25 ['AC_PINCTL_IN_EN']

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500



       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (1300)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 1300
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 1301 ); //   coef read 1301

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (1304)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 1304
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 1305 ); //   coef write 1305

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
//       snd_hda:     stream format 6 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000a7, 1319); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00a7 mute 1 gain 0x27 39
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000060b3); // 0x006360b3
//       snd_hda:     amp gain/mute 6 0x60b3 mute 1 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000a7, 1321); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00a7 mute 1 gain 0x27 39
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x000050b3); // 0x006350b3
//       snd_hda:     amp gain/mute 6 0x50b3 mute 1 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x000000b3, 1324); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00b3 mute 1 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
//       snd_hda:     amp gain/mute 6 0x6033 mute 0 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x000000b3, 1326); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x00b3 mute 1 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
//       snd_hda:     amp gain/mute 6 0x5033 mute 0 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1335); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
//       snd_hda:     stream format 10 [('CHAN', 4), ('RATE', 44100), ('BITS', 16), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1337); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1340); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1343); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1344); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1347); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1348); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (1351)
 //       { 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00009f9f }, //   coef read 1351
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00009f9f, 1352 ); //   coef read 1352

       // write node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0c04 arg 4 0x0000 (1355)
 //       { 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000 }, //   coef write 1355
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000, 1356 ); //   coef write 1356

       // read  node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x0000 arg 4 0x0000 data 0x00009f9f (1359)
 //       { 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00009f9f }, //   coef read 1359
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00009f9f, 1360 ); //   coef read 1360

       // write node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x1000 arg 4 0x0000 (1363)
 //       { 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000 }, //   coef write 1363
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000, 1364 ); //   coef write 1364


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1369); // 0x00af0600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1370); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1371); // 0x00bf0600
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1372); // 0x00bf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1373); // 0x00cf0600
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1374); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1375); // 0x00df0600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1376); // 0x00df0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x00000004 (1376)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000004 }, //   coef read 1376
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x00000004, 1377 ); //   coef read 1377

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (1380)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, //   coef write 1380
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 1381 ); //   coef write 1381

        snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);


       // read  node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0000 arg 4 0x0000 data 0x00000014 (1385)
 //       { 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000014 }, //   coef read 1385
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000014, 1386 ); //   coef read 1386

       // write node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0034 arg 4 0x0000 (1389)
 //       { 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000 }, //   coef write 1389
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000, 1390 ); //   coef write 1390

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1394); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1397); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1398); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1426); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1427); // 0x00cf0d00

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1429); // 0x00df0500

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1432); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1433); // 0x00df0d00

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1437); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1438); // 0x00df0d00


        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1443); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
//       snd_hda:     29 ['AC_PINCTL_OUT_EN']

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1461); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1464); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1465); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1469); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1470); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1473); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
//       snd_hda:     stream format 10 [('CHAN', 1), ('RATE', 48000), ('BITS', 8), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1475); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
//       snd_hda:     29 []


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1478); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
//       snd_hda:     stream format 10 [('CHAN', 4), ('RATE', 44100), ('BITS', 16), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1480); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0000 arg 4 0x0000 data 0x00000c04 (1483)
 //       { 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04 }, //   coef read 1483
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04, 1484 ); //   coef read 1484

       // write node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0c04 arg 4 0x0000 (1487)
 //       { 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000 }, //   coef write 1487
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000, 1488 ); //   coef write 1488

       // read  node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x0000 arg 4 0x0000 data 0x00001000 (1491)
 //       { 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000 }, //   coef read 1491
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000, 1492 ); //   coef read 1492

       // write node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x1000 arg 4 0x0000 (1495)
 //       { 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000 }, //   coef write 1495
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000, 1496 ); //   coef write 1496


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1501); // 0x00af0600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1502); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1503); // 0x00bf0600
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1504); // 0x00bf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1505); // 0x00cf0600
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1506); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1507); // 0x00df0600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1508); // 0x00df0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000000c4 (1508)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4 }, //   coef read 1508
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4, 1509 ); //   coef read 1509

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (1512)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, //   coef write 1512
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 1513 ); //   coef write 1513

        snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);


       // read  node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0000 arg 4 0x0000 data 0x00000034 (1517)
 //       { 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034 }, //   coef read 1517
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034, 1518 ); //   coef read 1518

       // write node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0034 arg 4 0x0000 (1521)
 //       { 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000 }, //   coef write 1521
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000, 1522 ); //   coef write 1522

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1526); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1529); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1530); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1557); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1558); // 0x00cf0d00

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1560); // 0x00df0500

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1563); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1564); // 0x00df0d00

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1568); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1569); // 0x00df0d00


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1572); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
//       snd_hda:     stream format 10 [('CHAN', 4), ('RATE', 44100), ('BITS', 16), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1574); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1577); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1580); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1581); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1584); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 1585); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0000 arg 4 0x0000 data 0x00000c04 (1588)
 //       { 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04 }, //   coef read 1588
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04, 1589 ); //   coef read 1589

       // write node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0c04 arg 4 0x0000 (1592)
 //       { 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000 }, //   coef write 1592
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000, 1593 ); //   coef write 1593

       // read  node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x0000 arg 4 0x0000 data 0x00001000 (1596)
 //       { 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000 }, //   coef read 1596
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000, 1597 ); //   coef read 1597

       // write node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x1000 arg 4 0x0000 (1600)
 //       { 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000 }, //   coef write 1600
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000, 1601 ); //   coef write 1601


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1606); // 0x00af0600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1607); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1608); // 0x00bf0600
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1609); // 0x00bf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1610); // 0x00cf0600
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1611); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 1612); // 0x00df0600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1613); // 0x00df0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000000c4 (1613)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4 }, //   coef read 1613
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4, 1614 ); //   coef read 1614

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (1617)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, //   coef write 1617
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 1618 ); //   coef write 1618

        snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);


       // read  node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0000 arg 4 0x0000 data 0x00000034 (1622)
 //       { 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034 }, //   coef read 1622
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034, 1623 ); //   coef read 1623

       // write node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0034 arg 4 0x0000 (1626)
 //       { 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000 }, //   coef write 1626
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000, 1627 ); //   coef write 1627

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1631); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1634); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 1635); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1663); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1664); // 0x00cf0d00

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1666); // 0x00df0500

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1669); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1670); // 0x00df0d00

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1674); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1675); // 0x00df0d00


        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 1680); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
//       snd_hda:     29 ['AC_PINCTL_OUT_EN']

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000000); // 0x00a70e00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1710); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 1713); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1714); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00a70600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 1718); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 1719); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 1722); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00000000); // 0x00a20000
//       snd_hda:     stream format 10 [('CHAN', 1), ('RATE', 48000), ('BITS', 8), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000040, 1724); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000000); // 0x01d70700
//       snd_hda:     29 []

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1733); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
//       snd_hda:     amp gain/mute 6 0x6033 mute 0 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1735); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
//       snd_hda:     amp gain/mute 6 0x5033 mute 0 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1744); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
//       snd_hda:     amp gain/mute 6 0x6033 mute 0 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1746); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
//       snd_hda:     amp gain/mute 6 0x5033 mute 0 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input

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
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37
//        vendor_verb
        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);


       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x8383 arg 4 0x0000 (1828)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000 }, //   coef write 1828
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000, 1829 ); //   coef write 1829

       // read  node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 data 0x00008383 (1832)
 //       { 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00008383 }, //   coef read 1832
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00008383, 1833 ); //   coef read 1833

       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x4383 arg 4 0x0000 (1836)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x4383, 0x00000000 }, //   coef write 1836
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x4383, 0x00000000, 1837 ); //   coef write 1837

       // write node 0x24 arg 1 0x0000 arg 2 0x001a arg 3 0x00f3 arg 4 0x0000 (1840)
 //       { 1, CS4208_VENDOR_NID, 0x001a, 0x00f3, 0x00000000 }, //   coef write 1840
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001a, 0x00f3, 0x00000000, 1841 ); //   coef write 1841

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
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37
//        vendor_verb
        snd_hda_codec_write(codec, 0x24, 0, 0x7f0, 0x00000000);


       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x8383 arg 4 0x0000 (1886)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000 }, //   coef write 1886
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x8383, 0x00000000, 1887 ); //   coef write 1887

       // read  node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x0000 arg 4 0x0000 data 0x00008383 (1890)
 //       { 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00008383 }, //   coef read 1890
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0019, 0x0000, 0x00008383, 1891 ); //   coef read 1891

       // write node 0x24 arg 1 0x0000 arg 2 0x0019 arg 3 0x4383 arg 4 0x0000 (1894)
 //       { 1, CS4208_VENDOR_NID, 0x0019, 0x4383, 0x00000000 }, //   coef write 1894
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0019, 0x4383, 0x00000000, 1895 ); //   coef write 1895

       // write node 0x24 arg 1 0x0000 arg 2 0x001a arg 3 0x00f3 arg 4 0x0000 (1898)
 //       { 1, CS4208_VENDOR_NID, 0x001a, 0x00f3, 0x00000000 }, //   coef write 1898
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x001a, 0x00f3, 0x00000000, 1899 ); //   coef write 1899

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
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
//       snd_hda:     amp gain/mute 6 0x6033 mute 0 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1933); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
//       snd_hda:     amp gain/mute 6 0x5033 mute 0 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00002000, 0x00000033, 1942); // 0x006b2000
//       snd_hda:     amp gain/mute 6 0x2000 index 0x00 left/right 1 left output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00006033); // 0x00636033
//       snd_hda:     amp gain/mute 6 0x6033 mute 0 gain 0x33 51 index 0x00 left 1 right 0 output 0 input 1 left   input
        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_AMP_GAIN_MUTE, 0x00000000, 0x00000033, 1944); // 0x006b0000
//       snd_hda:     amp gain/mute 6 0x0000 index 0x00 left/right 0 right output/input 0 input
//       snd_hda:     amp gain/mute 6 0x0033 mute 0 gain 0x33 51
        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_AMP_GAIN_MUTE, 0x00005033); // 0x00635033
//       snd_hda:     amp gain/mute 6 0x5033 mute 0 gain 0x33 51 index 0x00 left 0 right 1 output 0 input 1  right  input

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
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000000); // 0x00171500
//       snd_hda:     gpio data 1 0x00


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

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


       // read  node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 data 0x00000000 (2046)
 //       { 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef read 2046
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 2047 ); //   coef read 2047

       // write node 0x24 arg 1 0x0000 arg 2 0x0035 arg 3 0x0000 arg 4 0x0000 (2050)
 //       { 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000 }, //   coef write 2050
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0035, 0x0000, 0x00000000, 2051 ); //   coef write 2051

        snd_hda_codec_write(codec, 0x06, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004031); // 0x00624031
//       snd_hda:     stream format 6 [('CHAN', 2), ('RATE', 44100), ('BITS', 24), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00170503

	//printk("snd_hda_intel: setup_a1534 end");
	return 0;
}

static int play_a1534 (struct hda_codec *codec) {
        int retval;
        //printk("snd_hda_intel: play_a1534 begin");

        retval = snd_hda_codec_read_check(codec, 0x00, 0, AC_VERB_PARAMETERS, 0x00000000, 0x10134208, 1); // 0x000f0000

        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00170500


        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 9); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 10); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01


        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000001, 14); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000000); // 0x00a70d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_STREAM_FORMAT, 0x00004013); // 0x00a24013
//       snd_hda:     stream format 10 [('CHAN', 4), ('RATE', 44100), ('BITS', 16), ('RATE_MUL', 1), ('RATE_DIV', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 17); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00a70d01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 21); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 1)]

        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

       // read  node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0000 arg 4 0x0000 data 0x00000c04 (23)
 //       { 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04 }, //   coef read 23
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0004, 0x0000, 0x00000c04, 24 ); //   coef read 24

       // write node 0x24 arg 1 0x0000 arg 2 0x0004 arg 3 0x0c04 arg 4 0x0000 (27)
 //       { 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000 }, //   coef write 27
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0004, 0x0c04, 0x00000000, 28 ); //   coef write 28

       // read  node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x0000 arg 4 0x0000 data 0x00001000 (31)
 //       { 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000 }, //   coef read 31
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0005, 0x0000, 0x00001000, 32 ); //   coef read 32

       // write node 0x24 arg 1 0x0000 arg 2 0x0005 arg 3 0x1000 arg 4 0x0000 (35)
 //       { 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000 }, //   coef write 35
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0005, 0x1000, 0x00000000, 36 ); //   coef write 36


        snd_hda_codec_write(codec, 0x24, 0, AC_VERB_SET_PROC_STATE, 0x00000001); // 0x02470301

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000010, 41); // 0x00af0600
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 42); // 0x00af0500
        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00a70503

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 44); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000111, 45); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000010); // 0x00a70d10


        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 48); // 0x00bf0600
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 49); // 0x00bf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 50); // 0x00cf0600
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 51); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_CONV, 0x00000000, 0x00000000, 52); // 0x00df0600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 53); // 0x00df0500


       // read  node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x0000 arg 4 0x0000 data 0x000000c4 (53)
 //       { 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4 }, //   coef read 53
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0000, 0x0000, 0x000000c4, 54 ); //   coef read 54

       // write node 0x24 arg 1 0x0000 arg 2 0x0000 arg 3 0x00c4 arg 4 0x0000 (57)
 //       { 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000 }, //   coef write 57
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0000, 0x00c4, 0x00000000, 58 ); //   coef write 58

        snd_hda_codec_write(codec, 0x0a, 0, 0x7f0, 0x00000003);

       // read  node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0000 arg 4 0x0000 data 0x00000034 (62)
 //       { 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034 }, //   coef read 62
        snd_hda_coef_item(codec, 0, CS4208_VENDOR_NID, 0x0036, 0x0000, 0x00000034, 63 ); //   coef read 63

       // write node 0x24 arg 1 0x0000 arg 2 0x0036 arg 3 0x0034 arg 4 0x0000 (66)
 //       { 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000 }, //   coef write 66
        snd_hda_coef_item(codec, 1, CS4208_VENDOR_NID, 0x0036, 0x0034, 0x00000000, 67 ); //   coef write 67

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00a70500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 73); // 0x00af0500

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000110, 74); // 0x00af0d00

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 76); // 0x00af0500

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000010); // 0x00a70610
//       snd_hda:     conv stream channel map 10 [('CHAN', 0), ('STREAMID', 1)]

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 78); // 0x00bf0500

        snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00b70500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 81); // 0x00bf0500

        retval = snd_hda_codec_read_check(codec, 0x0b, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 82); // 0x00bf0d00

        snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00b70d01

        snd_hda_codec_write(codec, 0x0b, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00b70600
//       snd_hda:     conv stream channel map 11 [('CHAN', 0), ('STREAMID', 0)]

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
//       snd_hda:     conv stream channel map 12 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0c, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00c70503

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 98); // 0x00cf0500

        retval = snd_hda_codec_read_check(codec, 0x0c, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 99); // 0x00cf0d00

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 101); // 0x00df0500

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000000); // 0x00d70500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 104); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 105); // 0x00df0d00

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000001); // 0x00d70d01

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_CHANNEL_STREAMID, 0x00000000); // 0x00d70600
//       snd_hda:     conv stream channel map 13 [('CHAN', 0), ('STREAMID', 0)]

        snd_hda_codec_write(codec, 0x0d, 0, AC_VERB_SET_POWER_STATE, 0x00000003); // 0x00d70503

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 109); // 0x00df0500

        retval = snd_hda_codec_read_check(codec, 0x0d, 0, AC_VERB_GET_DIGI_CONVERT_1, 0x00000000, 0x00000000, 110); // 0x00df0d00


        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_CONNECT_SEL, 0x00000000); // 0x01d70100

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_2, 0x00000001); // 0x00a70e01

        snd_hda_codec_write(codec, 0x0a, 0, AC_VERB_SET_DIGI_CONVERT_1, 0x00000011); // 0x00a70d11

        retval = snd_hda_codec_read_check(codec, 0x1d, 0, AC_VERB_GET_PIN_WIDGET_CONTROL, 0x00000000, 0x00000000, 115); // 0x01df0700
        snd_hda_codec_write(codec, 0x1d, 0, AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00000040); // 0x01d70740
//       snd_hda:     29 ['AC_PINCTL_OUT_EN']


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DIRECTION, 0x00000031); // 0x00171731
//       snd_hda:     gpio direction 1 0x31 in in out out in in in out


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_DATA, 0x00000001); // 0x00171501
//       snd_hda:     gpio data 1 0x01


        snd_hda_codec_write(codec, codec->core.afg, 0, AC_VERB_SET_GPIO_MASK, 0x00000037); // 0x00171637
//       snd_hda:     gpio enable 1 0x37

        retval = snd_hda_codec_read_check(codec, 0x06, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000033, 129); // 0x006f0500
        retval = snd_hda_codec_read_check(codec, 0x0a, 0, AC_VERB_GET_POWER_STATE, 0x00000000, 0x00000000, 130); // 0x00af0500

        //printk("snd_hda_intel: play_a1534 end");
	return 0;
}

