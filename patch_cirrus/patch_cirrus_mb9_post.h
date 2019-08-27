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
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook open");
        int hp_pin_sense;
        unsigned int hp_pin;
        int err;
		hp_pin = 0x10; // HP pin is Node 0x10
		hp_pin_sense = snd_hda_jack_detect(codec, hp_pin);
		//printk("snd_hda_intel: hp_pin_sense: %d", hp_pin_sense);
		//hp_pin_sense = 0;
		//if (hp_pin_sense == 1) { // output to headphones
		//	printk("snd_hda_intel: playing to headphones");
		//}
		if (hp_pin_sense == 0) { // output to speakers
			//printk("snd_hda_intel: playing to speakers");
        		err = play_a1534(codec);
		}

		//if (!spec->play_init) {
		if (1) {
			//int power_chk = 0;
			struct timespec curtim;
			getnstimeofday(&curtim);
			spec->first_play_time.tv_sec = curtim.tv_sec;
			//cs_4208_play_setup(codec);
			//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD setup play called");
			spec->play_init = 1;
			spec->playing = 0;
		}

		// we need to force the stream to be re-set here
		// problem is it appears hda_codec caches the stream format and id and only updates if changed
		// and there doesnt seem to be a good way to force an update
		// problem - the get_hda_cvt_setup function is local to hda_codec

		// this routine doesnt seem to be nid specific - so explicitly fix the known nids here

		p = get_hda_cvt_setup_4208(codec, 0x02);

		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD cvt pointer 0x02 %p",p);

		p->stream_tag = 0;
		p->channel_id = 0;
		p->format_id = 0;

		p = get_hda_cvt_setup_4208(codec, 0x02);

		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD cvt pointer 0x03 %p",p);

		p->stream_tag = 0;
		p->channel_id = 0;
		p->format_id = 0;

		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook end");
	} else if (action == HDA_GEN_PCM_ACT_PREPARE) {
		struct timespec curtim;
		getnstimeofday(&curtim);
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD HOOK PREPARE init %d last %ld cur %ld",spec->play_init,spec->last_play_time.tv_sec,curtim.tv_sec);
		//if (spec->play_init && curtim.tv_sec > (spec->first_play_time.tv_sec + 0))
		//if (spec->play_init) {
		if (1) {
			int power_chk = 0;
        		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
			//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD power check 0x01 2 %d", power_chk);
			spec->last_play_time.tv_sec = curtim.tv_sec;
			spec->playing = 1;
		}

		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD HOOK PREPARE end");
	} else if (action == HDA_GEN_PCM_ACT_CLEANUP) {
		int power_chk = 0;
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD HOOK CLEANUP");
		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD power check 0x01 3 %d", power_chk);
		//if (spec->playing) {
			//cs_4208_play_cleanup(codec);
			printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD done play down");
			spec->playing = 0;
		//}
		//cs_4208_play_cleanup(codec);
		power_chk = snd_hda_codec_read(codec, codec->core.afg, 0, AC_VERB_GET_POWER_STATE, 0);
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD power check 0x01 4 %d", power_chk);
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook BAD HOOK CLEANUP end");
	}
	//} else if (action == HDA_GEN_PCM_ACT_CLOSE) {
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook close");
		//printk("snd_hda_intel: command nid cs_4208_playback_pcm_hook end");
	//}

}

static int cs_4208_build_controls_explicit(struct hda_codec *codec)
{
	//printk("snd_hda_intel: cs_4208_build_controls_explicit\n");
	//printk("snd_hda_intel: end cs_4208_build_controls_explicit\n");
	return 0;
}

int cs_4208_build_pcms_explicit(struct hda_codec *codec)
{
	int retval;

	struct cs_spec *spec = codec->spec;
	struct hda_gen_spec *gen_spec = &(spec->gen);
	struct hda_pcm *info;

        //printk("snd_hda_intel: cs_4208_build_pcms_explicit\n");

	//retval =  snd_hda_gen_build_pcms(codec);

	cs_4208_fill_pcm_stream_name(gen_spec->stream_name_analog,
					sizeof(gen_spec->stream_name_analog),
					" Analog", codec->core.chip_name);
	info = snd_hda_codec_pcm_new(codec, "%s", gen_spec->stream_name_analog);
	if (!info)
		return -ENOMEM;
	gen_spec->pcm_rec[0] = info;

	info->stream[SNDRV_PCM_STREAM_PLAYBACK] = cs4208_pcm_analog_playback;
	info->stream[SNDRV_PCM_STREAM_PLAYBACK].nid = 0x04; // 0x04 is for speakers 0x02 is for headphones
	info->stream[SNDRV_PCM_STREAM_PLAYBACK].channels_max = 4;

	info->pcm_type = HDA_PCM_TYPE_AUDIO;

	retval = 0;

	//printk("snd_hda_intel: end cs_4208_build_pcms_explicit\n");
	return retval;
}
void cs_4208_jack_unsol_event(struct hda_codec *codec, unsigned int res)
{
	struct hda_jack_tbl *event;
	int tag = (res >> AC_UNSOL_RES_TAG_SHIFT) & 0x7f;

	dev_info(hda_codec_dev(codec), "cs_4208_jack_unsol_event 0x%08x tag 0x%02x\n",res,tag);

	event = snd_hda_jack_tbl_get_from_tag(codec, tag);
        if (!event)
		return;
	event->jack_dirty = 1;

	//call_jack_callback(codec, event);
	snd_hda_jack_report_sync(codec);
}

#define cs_4208_free            snd_hda_gen_free

static int cs_4208_init_explicit(struct hda_codec *codec)
{
	//struct cs_spec *spec = codec->spec;

	//printk("snd_hda_intel: cs_4208_init_explicit enter\n");
	//printk("snd_hda_intel: cs_4208_init snd_hda_gen_init NOT CALLED\n");
	//printk("snd_hda_intel: cs_4208_init_explicit end\n");

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
	.unsol_event = snd_hda_jack_unsol_event, //cs_4208_jack_unsol_event,
//#ifdef UNDEF_CONFIG_PM
//      .suspend = cs_4208_suspend,
//      .resume = cs_4208_resume,
//#endif
};
