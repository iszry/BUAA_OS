#include <asm/asm.h>
#include <pmap.h>
#include <env.h>
#include <printf.h>
#include <kclock.h>
#include <trap.h>

void mips_init() {
	printf("init.c:\tmips_init() is called\n");
	mips_detect_memory();

	mips_vm_init();
	//printf("start page_init\n");
	page_init();
	//printf("start env_init\n");
	env_init();
	//printf("start ENV_CREATE\n");
	ENV_CREATE(user_thread);
	//ENV_CREATE(user_tltest);
	//ENV_CREATE(user_fktest);
	//ENV_CREATE(user_pingpong);

	trap_init();
	kclock_init();


	while(1);
	panic("init.c:\tend of mips_init() reached!");
}

void bcopy(const void *src, void *dst, size_t len) {
	void *max = dst + len;

	// copy machine words while possible
	while (dst + 3 < max) {
		*(int *)dst = *(int *)src;
		dst+=4;
		src+=4;
	}

	// finish the remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst = *(char *)src;
		dst+=1;
		src+=1;
	}
}

void bzero(void *b, size_t len) {
	void *max = b + len;

	// fill machine words while possible
	while (b + 3 < max) {
		*(int *)b = 0;
		b += 4;
	}

	// finish the remaining 0-3 bytes
	while (b < max) {
		*(char *)b++ = 0;
	}
}
