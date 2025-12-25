#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "arpsniffer.h"

int main(int argc, char *argv[]) {

    if (geteuid() != 0) {
        fprintf(stderr, "This tool must be run as root.\n");
        return 1;
    }

    if (argc < 2 ||
        strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0) {

        print_version();
        print_help(argv[0]);
        return 0;
    }

    if (strcmp(argv[1], "-l") == 0 ||
        strcmp(argv[1], "--lookup") == 0) {

        print_available_interfaces();
        return 0;
    }

    if (strcmp(argv[1], "-v") == 0 ||
        strcmp(argv[1], "--version") == 0) {

        print_version();
        return 0;
    }

    if (strcmp(argv[1], "-i") == 0 ||
        strcmp(argv[1], "--interface") == 0) {

        if (argc < 3) {
            fprintf(stderr, "Error: interface not provided\n");
            print_available_interfaces();
            return 1;
        }

        return sniff_arp(argv[2]);
    }

    fprintf(stderr, "Unknown option: %s\n", argv[1]);
    print_help(argv[0]);
    return 1;
}
