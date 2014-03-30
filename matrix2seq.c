#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

char *progname;

void usage()
{
	fprintf(stderr, "usage: %s [-i input-file] [-o output-file]\n", progname);

	return;
}

int main(int argc, char *argv[])
{
	int c;
	char *input_filename=NULL, *output_filename=NULL;
	FILE *input_fp, *output_fp;
	char buf[0xffff];

	progname=argv[0];

        while((c=getopt(argc, argv, "h?i:o:"))!=-1)
                switch(c){
                        case 'i':
                                input_filename=optarg;
                                break;
                        case 'o':
                                output_filename=optarg;
                                break;
			case 'h':
                        case '?':
				usage();
				return 0;
                        default:
				fprintf(stderr, "%s: unknown option %c specified\n", progname, c);
				usage();
				exit(EXIT_FAILURE);
                }

        if(optind!=argc){
                fprintf(stderr, "%s: extra arguments specified\n", progname);
		usage();
		exit(EXIT_FAILURE);
        }


	if(!input_filename)
		input_fp=stdin;
	else if((!strcmp(input_filename, "-"))||(!strcmp(input_filename, "")))
		input_fp=stdin;
	else{
		input_fp=fopen(input_filename, "r");
		if(!input_fp){
			fprintf(stderr, "%s: fopen: %s: %s\n", progname, input_filename, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	if(!output_filename)
		output_fp=stdout;
	else if((!strcmp(output_filename, "-"))||(!strcmp(output_filename, "")))
		output_fp=stdout;
	else{
		output_fp=fopen(output_filename, "w");
		if(!output_fp){
			fprintf(stderr, "%s: fopen: %s: %s\n", progname, output_filename, strerror(errno));
			exit(EXIT_FAILURE);
		}
	}

	/* skip leading - */
	do
		fgets(buf, 0xffff, input_fp);
	while(buf[0]=='-');
	while(!feof(input_fp)){
		buf[strlen(buf)-1]='\0';
		fputs(buf, output_fp);
		fputc(' ', output_fp);
		fgets(buf, 0xffff, input_fp);
		if(buf[0]=='-'){
			fputc('\n', output_fp);
			do
				fgets(buf, 0xffff, input_fp);
			while(buf[0]=='-');
		}
	}

	return 0;
}
