int cpuid_check(){

        /* CPUID is an x86 specific interface. */
	#if defined(__i386__) || defined(__x86_64__)

        static const struct {
                const char *cpuid;
                int id;
        } cpuid_vendor_table[] = {
                { "XenVMMXenVMM", VIRTUALIZATION_XEN       },
                { "KVMKVMKVM",    VIRTUALIZATION_KVM       },
                { "VMwareVMware", VIRTUALIZATION_VMWARE    },
                { "Microsoft Hv", VIRTUALIZATION_MICROSOFT },
        };

        uint32_t eax, ecx;
        bool hypervisor;

	#if defined (__i386__)
	#define REG_a "eax"
	#define REG_b "ebx"
	#elif defined (__amd64__)
	#define REG_a "rax"
	#define REG_b "rbx"
	#endif

        eax = 1;
        __asm__ __volatile__ (
                "  push %%"REG_b"         \n\t"
                "  cpuid                  \n\t"
                "  pop %%"REG_b"          \n\t"
                : "=a" (eax), "=c" (ecx)
                : "0" (eax)
        );

        hypervisor = !!(ecx & 0x80000000U); // 2^32

        if (hypervisor) {
                union {
                        uint32_t sig32[3];
                        char text[13];
                } sig = {};
                unsigned j;

                /* There is a hypervisor, see what it is */
                eax = 0x40000000U; // Hypervisor CPUID Information Leaf
                __asm__ __volatile__ (
                        "  push %%"REG_b"         \n\t"
                        "  cpuid                  \n\t"
                        "  mov %%ebx, %1          \n\t"
                        "  pop %%"REG_b"          \n\t"
                        : "=a" (eax), "=r" (sig.sig32[0]), 
			  "=c" (sig.sig32[1]), "=d" (sig.sig32[2])
                        : "0" (eax)
                );
                for (j = 0; j < ELEMENTSOF(cpuid_vendor_table); j++){
                        if (strcmp(sig.text, cpuid_vendor_table[j].cpuid) == 0){
				printf("No running in ");
				if (strcmp("XenVMMXenVMM", cpuid_vendor_table[j].cpuid) == 0){
					printf("Xen Hypervisor allowed!\n");
				}
				else if (strcmp("KVMKVMKVM", cpuid_vendor_table[j].cpuid) == 0){
					printf("KVM allowed!\n");
				}
				else if (strcmp("VMwareVMware", cpuid_vendor_table[j].cpuid) == 0){
					printf("VMWare allowed!\n");
				}
				else if (strcmp("Microsoft Hv", cpuid_vendor_table[j].cpuid) == 0){
					printf("Microsoft Hypervisor allowed!\n");
				}
                                return 1;
			}
		}
                return 1;
        }
#endif

        return 0;
}
