static int cs_4208_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
					struct hda_codec *codec,
					unsigned int stream_tag,
					unsigned int format,
					struct snd_pcm_substream *substream)
{
	struct cs_spec *spec = codec->spec;
	codec_dbg(codec, "cs_4208_playback_pcm_prepare enter\n");
	snd_hda_codec_setup_stream(codec, hinfo->nid, stream_tag, 0, format);

	if (spec->gen.pcm_playback_hook)
		spec->gen.pcm_playback_hook(hinfo, codec, substream, HDA_GEN_PCM_ACT_PREPARE);

	codec_dbg(codec, "cs_4208_playback_pcm_prepare end\n");

	return 0;
}

static int cs_4208_playback_pcm_cleanup(struct hda_pcm_stream *hinfo,
					struct hda_codec *codec,
					struct snd_pcm_substream *substream)
{
	//struct cs_spec *spec = codec->spec;
	codec_dbg(codec, "cs_4208_playback_pcm_cleanup enter\n");
	snd_hda_codec_cleanup_stream(codec, hinfo->nid);
	codec_dbg(codec, "cs_4208_playback_pcm_cleanup exit\n");

	return 0;
}

// this is very hacky but until get more understanding of what we can do with the 4208 setup
// re-define these from hda_codec.c here
// NOTA BENE - need to check this is consistent with any hda_codec.c updates!!

/*
 * audio-converter setup caches
 */
struct hda_cvt_setup {
	hda_nid_t nid;
	u8 stream_tag;
	u8 channel_id;
	u16 format_id;
	unsigned char active;   /* cvt is currently used */
	unsigned char dirty;    /* setups should be cleared */
};
/* get or create a cache entry for the given audio converter NID */
static struct hda_cvt_setup *
get_hda_cvt_setup_4208(struct hda_codec *codec, hda_nid_t nid)
{
	struct hda_cvt_setup *p;
	int i;

	for (i = 0; i < codec->cvt_setups.used; i++) {
		p = snd_array_elem(&codec->cvt_setups, i);
		if (p->nid == nid)
			return p;
	}
	p = snd_array_new(&codec->cvt_setups);
	if (p)
		p->nid = nid;
	return p;
}

static int cs_4208_playback_pcm_open(struct hda_pcm_stream *hinfo,
				     struct hda_codec *codec,
				     struct snd_pcm_substream *substream)
{
	int err;
	int hp_pin_sense;
	unsigned int hp_pin;
	//printk("snd_hda_intel: playback_pcm_open hook");
	codec_dbg(codec, "playback_pcm_open nid 0x%02x rates 0x%08x formats 0x%016llx\n",hinfo->nid,hinfo->rates,hinfo->formats);
		hp_pin = 0x10; // HP pin is Node 0x10
		//hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
                hp_pin_sense = 0; //HP not working at the moment, disabling for now 
		//printk("snd_hda_intel: hp_pin_sense: %d", hp_pin_sense);
		//if (hp_pin_sense == 1) { // output to headphones
			//printk("snd_hda_intel: playing to headphones");
			//err = headphones_a1534(codec);
		//}  else {
		if (hp_pin_sense == 0) { // output to speakers
			//printk("snd_hda_intel: playing to speakers");
			err = play_a1534(codec);
		}
	
	//call_pcm_playback_hook(hinfo, codec, substream, HDA_GEN_PCM_ACT_OPEN);

	//mutex_lock(&gen_spec->pcm_mutex);
	//err = snd_hda_multi_out_analog_open(codec,
						//&spec->multiout, substream,
						//hinfo);
	//if (!err) {
		//spec->active_streams |= 1 << STREAM_MULTI_OUT;
		//call_pcm_playback_hook(hinfo, codec, substream,
					//HDA_GEN_PCM_ACT_OPEN);
	//}
	//mutex_unlock(&gen_spec->pcm_mutex);
	return 0;
}

static const struct hda_pcm_stream cs4208_pcm_analog_playback = {
	.substreams = 1,
	.channels_min = 2,
	.channels_max = 4,
	.rates = SNDRV_PCM_RATE_44100,
	.formats = SNDRV_PCM_FMTBIT_S16_LE, //|SNDRV_PCM_FMTBIT_S24_LE
	.maxbps = 16, // 32 before
	.ops = {
 		.open = cs_4208_playback_pcm_open,
		.prepare = cs_4208_playback_pcm_prepare,
		.cleanup = cs_4208_playback_pcm_cleanup,
	},
};

static void cs_4208_fill_pcm_stream_name(char *str, size_t len, const char *sfx,
					 const char *chip_name)
{
	char *p;

	if (*str)
		return;
	strlcpy(str, chip_name, len);

	/* drop non-alnum chars after a space */
	for (p = strchr(str, ' '); p; p = strchr(p + 1, ' ')) {
		if (!isalnum(p[1])) {
			*p = 0;
			break;
		}
	}
	strlcat(str, sfx, len);
}
