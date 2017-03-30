#define VMWARE_HYPERVISOR_MAGIC 	0x564D5868
#define VMWARE_HYPERVISOR_PORT  	0x5658
#define VMWARE_PORT_CMD_GETVERSION      10
#define UINT_MAX 			0xFFFFFFFF

#define VMWARE_PORT(	eax, ebx, ecx, edx, a, b, c, d 				\
		   ){ 								\
			__asm__("inl %%dx" 					\
				:"=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) \
				:"0"(a), "1"(b), "2"(c), "3"(d) ); 		\
		    }

int hypervisor_port_check(){
        unsigned int eax, ebx, ecx, edx;
	VMWARE_PORT(&eax, &ebx, &ecx, &edx, VMWARE_HYPERVISOR_MAGIC, UINT_MAX, VMWARE_PORT_CMD_GETVERSION, VMWARE_HYPERVISOR_PORT);
	if (ebx == VMWARE_HYPERVISOR_MAGIC) { return 1; } else { return 0; }
}
