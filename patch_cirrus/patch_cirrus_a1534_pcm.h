/* ---- CS4208 live headphone<->speaker auto-switch state ----
 * The captured macOS sequences (play_a1534 / headphones_a1534) only get run at
 * PCM-open time. To switch a *running* stream when the jack is plugged/unplugged
 * we cache the live stream tag+format here and re-point the DMA stream onto the
 * correct converter from the unsolicited jack handler below. */
static unsigned int cs4208_cur_stream_tag;	/* running PCM stream tag (0 = none) */
static unsigned int cs4208_cur_format;		/* running PCM HDA format word */
static int cs4208_cur_hp = -1;			/* last HP jack state (-1 = unknown) */
/* cs_4208_hp_jack_callback is defined further down, after get_hda_cvt_setup_4208,
 * which it needs to invalidate the converter format cache. */

static int cs_4208_playback_pcm_prepare(struct hda_pcm_stream *hinfo,
					struct hda_codec *codec,
					unsigned int stream_tag,
					unsigned int format,
					struct snd_pcm_substream *substream)
{
	struct cs_spec *spec = codec->spec;
	codec_dbg(codec, "cs_4208_playback_pcm_prepare start");
	cs4208_cur_stream_tag = stream_tag;	/* cache for live jack re-pointing */
	cs4208_cur_format = format;
	snd_hda_codec_setup_stream(codec, hinfo->nid, stream_tag, 0, format);

	if (spec->gen.pcm_playback_hook)
		spec->gen.pcm_playback_hook(hinfo, codec, substream, HDA_GEN_PCM_ACT_PREPARE);

	codec_dbg(codec, "cs_4208_playback_pcm_prepare end");

	return 0;
}

static int cs_4208_playback_pcm_cleanup(struct hda_pcm_stream *hinfo,
					struct hda_codec *codec,
					struct snd_pcm_substream *substream)
{
	//struct cs_spec *spec = codec->spec;
	codec_dbg(codec, "cs_4208_playback_pcm_cleanup start");
	cs4208_cur_stream_tag = 0;	/* no running stream to re-point on jack change */
	snd_hda_codec_cleanup_stream(codec, hinfo->nid);
	codec_dbg(codec, "cs_4208_playback_pcm_cleanup end");

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
	codec_dbg(codec, "cs_4208_playback_pcm_open start");
	codec_dbg(codec, "playback_pcm_open nid 0x%02x rates 0x%08x formats 0x%016llx\n",hinfo->nid,hinfo->rates,hinfo->formats);
		hp_pin = 0x10; // HP pin is Node 0x10
		hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
		cs4208_cur_hp = hp_pin_sense; // keep jack-callback in sync with open-path routing
		codec_dbg(codec, "cs_4208_playback_pcm_open hp_pin_sense: %d", hp_pin_sense);
		if (hp_pin_sense) { // output to headphones
			codec_dbg(codec, "cs_4208_playback_pcm_open playing to headphones");
			hinfo->nid = 0x02; // headphone converter (DAC) -> HP pin 0x10
			err = headphones_a1534(codec);
		} else { // output to speakers
			codec_dbg(codec, "cs_4208_playback_pcm_open playing to speakers");
			hinfo->nid = 0x04; // speaker converter (DAC) -> speaker pin 0x1d
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
	codec_dbg(codec, "cs_4208_playback_pcm_open end");
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
	strscpy(str, chip_name, len);

	/* drop non-alnum chars after a space */
	for (p = strchr(str, ' '); p; p = strchr(p + 1, ' ')) {
		if (!isalnum(p[1])) {
			*p = 0;
			break;
		}
	}
	strscpy(str, sfx, len);
}

void cs_4208_playback_pcm_hook(struct hda_pcm_stream *hinfo, struct hda_codec *codec, struct snd_pcm_substream *substream, int action)
{

	struct cs_spec *spec = codec->spec;

	// so finally getting a handle on ordering here
	// we need to do the OSX setup in the OPEN section
	// as the generic hda format and stream setup is done BEFORE the PREPARE hook
	// (theres a good chance we only need to do this once at least as long as machine doesnt sleep)
	// (or we could just override the prepare function completely)
	// I now think the noise was caused by mis-match between the stream format and the nid setup format
	// (because the generic setup was done before the OSX setup and the actual streamed format is slightly different)
	// (the hda documentation says these really need to match)
	// It appears the 4208 setup can handle at least some differences in the stream format
	// certainly seems to handle S24_LE or S32_LE differences

	
	if (action == HDA_GEN_PCM_ACT_OPEN) {
		struct hda_cvt_setup *p = NULL;
        	int hp_pin_sense;
        	unsigned int hp_pin;
        	int err;
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_OPEN start");

		hp_pin = 0x10; // HP pin is Node 0x10
		hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
		codec_dbg(codec, "cs_4208_playback_pcm_hook hp_pin_sense: %d", hp_pin_sense);
		//hp_pin_sense = 0;
		//if (hp_pin_sense == 1) { // output to headphones
		//	codec_dbg(codec, "cs_4208_playback_pcm_hook playing to headphones");
		//}
		if (hp_pin_sense == 0) { // output to speakers
			//codec_dbg(codec, "cs_4208_playback_pcm_hook playing to speakers");
        		err = play_a1534(codec);
		}

		//if (!spec->play_init) {
		if (1) {
			//int power_chk = 0;
			struct timespec64 curtim;
			ktime_get_real_ts64(&curtim);
			spec->first_play_time.tv_sec = curtim.tv_sec;
			//cs_4208_play_setup(codec);
			codec_dbg(codec, "cs_4208_playback_pcm_hook BAD setup play called");
			spec->play_init = 1;
			spec->playing = 0;
		}

		// we need to force the stream to be re-set here
		// problem is it appears hda_codec caches the stream format and id and only updates if changed
		// and there doesnt seem to be a good way to force an update
		// problem - the get_hda_cvt_setup function is local to hda_codec

		// this routine doesnt seem to be nid specific - so explicitly fix the known nids here

		p = get_hda_cvt_setup_4208(codec, 0x02);
		codec_dbg(codec, "cs_4208_playback_pcm_hook BAD cvt pointer 0x02 %p", p);

		p->stream_tag = 0;
		p->channel_id = 0;
		p->format_id = 0;

		p = get_hda_cvt_setup_4208(codec, 0x02);

		codec_dbg(codec, "cs_4208_playback_pcm_hook BAD cvt pointer 0x03 %p", p);

		p->stream_tag = 0;
		p->channel_id = 0;
		p->format_id = 0;

		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_OPEN end");

	} else if (action == HDA_GEN_PCM_ACT_PREPARE) {
		struct timespec64 curtim;
		ktime_get_real_ts64(&curtim);
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_PREPARE start");
		codec_dbg(codec, "cs_4208_playback_pcm_hook BAD HOOK PREPARE init %d last %lld cur %lld", spec->play_init,spec->last_play_time.tv_sec,curtim.tv_sec);
		//if (spec->play_init && curtim.tv_sec > (spec->first_play_time.tv_sec + 0))
		//if (spec->play_init) {
		if (1) {
			int power_chk = 0;
        		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
			codec_dbg(codec, "cs_4208_playback_pcm_hook BAD power check 0x01 2 %d", power_chk); 
			spec->last_play_time.tv_sec = curtim.tv_sec;
			spec->playing = 1;
		}

		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_PREPARE end");
	} else if (action == HDA_GEN_PCM_ACT_CLEANUP) {
		int power_chk = 0;
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_CLEANUP start");
		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
		codec_dbg(codec, "cs_4208_playback_pcm_hook BAD power check 0x01 3 %d", power_chk);
		//if (spec->playing) {
			//cs_4208_play_cleanup(codec);
			codec_dbg(codec, "cs_4208_playback_pcm_hook BAD done play down");
			spec->playing = 0;
		//}
		//cs_4208_play_cleanup(codec);
		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
		codec_dbg(codec, "cs_4208_playback_pcm_hook BAD power check 0x01 4 %d", power_chk);
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_CLEANUP end");
	}
	//} else if (action == HDA_GEN_PCM_ACT_CLOSE) {
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_CLOSE start");
		codec_dbg(codec, "cs_4208_playback_pcm_hook HDA_GEN_PCM_ACT_CLOSE end");
	//}
	codec_dbg(codec, "cs_4208_playback_pcm_hook end");

}

static int cs_4208_build_controls_explicit(struct hda_codec *codec)
{
	int retval;
	codec_dbg(codec, "cs_4208_build_controls_explicit start");

	// Without this, no capture / mic mixer controls get created, ALSA never
	// selects an ADC for input, so the capture stream stays silent. We let
	// the generic builder create the standard set; speaker output still goes
	// through our custom playback PCM regardless.
	retval = snd_hda_gen_build_controls(codec);
	if (retval < 0)
		return retval;

	codec_dbg(codec, "cs_4208_build_controls_explicit end");
	return 0;
}

int cs_4208_build_pcms_explicit(struct hda_codec *codec)
{
	int retval;

	struct cs_spec *spec = codec->spec;
	struct hda_gen_spec *gen_spec = &(spec->gen);
	struct hda_pcm *info;

	codec_dbg(codec, "cs_4208_build_pcms_explicit start");

	// Let the generic builder set up everything (including the capture stream
	// for the internal/external mic). Without this we get no capture device
	// at all — speakers may work but mic is dead. We then override only the
	// analog playback stream below with the leifliddy custom config.
	retval = snd_hda_gen_build_pcms(codec);
	if (retval < 0)
		return retval;

	cs_4208_fill_pcm_stream_name(gen_spec->stream_name_analog,
					sizeof(gen_spec->stream_name_analog),
					" Analog", codec->core.chip_name);
	info = gen_spec->pcm_rec[0];
	if (!info)
		return -ENOMEM;

	info->stream[SNDRV_PCM_STREAM_PLAYBACK] = cs4208_pcm_analog_playback;
	info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = 0x04; // 0x04 is for speakers 0x02 is for headphones
	info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max = 4;

	info->pcm_type = HDA_PCM_TYPE_AUDIO;

	codec_dbg(codec, "cs_4208_build_pcms_explicit end");
	return retval;
}

/*
void cs_4208_jack_unsol_event(struct hda_codec *codec, unsigned int res)
{
	struct hda_jack_tbl *event;
	int tag = (res >> AC_UNSOL_RES_TAG_SHIFT) & 0x7f;

	dev_info(hda_codec_dev(codec), "cs_4208_jack_unsol_event 0x%08x tag 0x%02x\n",res,tag);

	event = snd_hda_jack_tbl_get_from_tag(codec, tag, 0);
        if (!event)
		return;
	event->jack_dirty = 1;

	//call_jack_callback(codec, event);
	snd_hda_jack_report_sync(codec);
}
*/

#define cs_4208_free            snd_hda_gen_free

/* Unsolicited HP-jack handler: re-route speaker<->headphone for a *running*
 * stream. Defined here (not at the top) because it needs get_hda_cvt_setup_4208
 * to invalidate the converter format cache before re-pointing the stream. */
static void cs_4208_hp_jack_callback(struct hda_codec *codec,
				     struct hda_jack_callback *cb)
{
	struct hda_cvt_setup *p;
	int hp = snd_hda_jack_detect(codec, 0x10);

	codec_dbg(codec, "cs_4208_hp_jack_callback hp=%d (prev=%d) tag=%u",
		  hp, cs4208_cur_hp, cs4208_cur_stream_tag);
	if (hp == cs4208_cur_hp)
		return;
	cs4208_cur_hp = hp;

	if (hp) {				/* headphones plugged */
		headphones_a1534(codec);
		if (cs4208_cur_stream_tag) {
			/* Invalidate the cached converter setup so setup_stream
			 * actually reprograms the live format - otherwise 0x02
			 * keeps headphones_a1534()'s hardcoded format and plays
			 * loud garbage until PipeWire's own reopen fixes it. */
			p = get_hda_cvt_setup_4208(codec, 0x02);
			p->stream_tag = 0; p->channel_id = 0; p->format_id = 0;
			snd_hda_codec_setup_stream(codec, 0x02,
						   cs4208_cur_stream_tag, 0,
						   cs4208_cur_format);
		}
		/* Silence the speaker by disabling its output pin (0x1d) without
		 * touching the converters/stream the headphones now use.
		 * play_a1534() re-enables 0x1d on the way back to speakers. */
		snd_hda_codec_write(codec, 0x1d, 0,
				    AC_VERB_SET_PIN_WIDGET_CONTROL, 0x00);
	} else {				/* back to speakers */
		play_a1534(codec);
		if (cs4208_cur_stream_tag) {
			p = get_hda_cvt_setup_4208(codec, 0x04);
			p->stream_tag = 0; p->channel_id = 0; p->format_id = 0;
			snd_hda_codec_setup_stream(codec, 0x04,
						   cs4208_cur_stream_tag, 0,
						   cs4208_cur_format);
		}
	}
}

static int cs_4208_init_explicit(struct hda_codec *codec)
{
	int retval;
	codec_dbg(codec, "cs_4208_init_explicit start");

	// Without calling the generic init, the codec's input path (ADC mux
	// selection, mic pin enables, capture amp defaults) never gets set up,
	// so even with capture controls present recording stays silent.
	// snd_hda_gen_init configures all input/output paths from the auto-config.
	retval = snd_hda_gen_init(codec);
	if (retval < 0)
		return retval;

	/* Enable live speaker<->headphone switching: fire cs_4208_hp_jack_callback
	 * on every plug/unplug interrupt from the HP pin (0x10). */
	cs4208_cur_hp = -1;
	snd_hda_jack_detect_enable_callback(codec, 0x10, cs_4208_hp_jack_callback);

	codec_dbg(codec, "cs_4208_init_explicit end");
	return 0;
}

static void cs_4208_free_explicit(struct hda_codec *codec)
{
	kfree(codec->spec);
}

// real def is CONFIG_PM
static const struct hda_codec_ops cs_4208_patch_ops_explicit = {
	.build_controls = cs_4208_build_controls_explicit,
	.build_pcms = cs_4208_build_pcms_explicit,
	.init = cs_4208_init_explicit,
	.free = cs_4208_free_explicit,
	.unsol_event = snd_hda_jack_unsol_event,
//#ifdef UNDEF_CONFIG_PM
//      .suspend = cs_4208_suspend,
//      .resume = cs_4208_resume,
//#endif
};
