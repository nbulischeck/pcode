int read_one_line_file(const char *fn, char **line) {
	FILE *read;
	read = fopen(fn, "r");
	if ( read == NULL ) { return 0; }
	fseeko(read, 0, SEEK_END);
	if ( ftello(read) == 0 ){ printf("Blank file!\n"); return 0; }
	rewind(read);
	fscanf(read, "%ms", line);
	fclose(read);
	return 1;
}


int dmi_check(){

	#if defined(__i386__) || defined(__x86_64__) || defined(__arm__) || defined(__aarch64__)

        unsigned i;

        static const char *const dmi_vendors[] = {
                "/sys/class/dmi/id/product_name",
                "/sys/class/dmi/id/sys_vendor",
                "/sys/class/dmi/id/board_vendor",
                "/sys/class/dmi/id/bios_vendor"
        };

        static const struct {
                const char *vendor;
                int id;
        } dmi_vendor_table[] = {
                { "KVM",           VIRTUALIZATION_KVM       },
                { "QEMU",          VIRTUALIZATION_QEMU      },
                { "VMware",        VIRTUALIZATION_VMWARE    },
                { "VMW",           VIRTUALIZATION_VMWARE    },
                { "innotek GmbH",  VIRTUALIZATION_ORACLE    },
		{ "VirtualBox",    VIRTUALIZATION_ORACLE    },
                { "Xen",           VIRTUALIZATION_XEN       },
                { "Bochs",         VIRTUALIZATION_BOCHS     },
                { "Parallels",     VIRTUALIZATION_PARALLELS },
        };

        for (i = 0; i < ELEMENTSOF(dmi_vendors); i++) {
                char *s = NULL;
                unsigned j;

                read_one_line_file(dmi_vendors[i], &s);

                for (j = 0; j < ELEMENTSOF(dmi_vendor_table); j++){
                        if ( (strcmp(s, dmi_vendor_table[j].vendor) == 0) )
                                return 1;
		}
        }

	return 0;
#endif

}
