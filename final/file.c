void *fileFunc (void *x_void_ptr) {

	while(1) {
		//printf(".\n");
		FILE* f = fopen("/home/maxg/dev/tmp/final/xxx", "rb");
		int i;
    		size_t lSize;
    		fseek (f , 0 , SEEK_END);
    		lSize = ftell (f);
    		rewind (f);

		uint8_t buf[2];

    		for (i=0; i<lSize/2; i++) {
			fread (&buf, 1, 2, f);
			gui_params[buf[0]] = buf[1];
		}
   		close(f);

		delay(500);

		f = fopen("/home/maxg/dev/tmp/final/yyy", "r");
		fread (&buf, 1, 1, f);

		//printf("%c %c\n", buf[0], NEW_PATCH);
		NEW_PATCH = buf[0];
		close(f);
		delay(500);

	}
}


