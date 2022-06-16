#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>


/* A process is a single process.  */
typedef struct process
{
	struct process *next;       /* next process in pipeline */
	char **argv;                /* for exec */
	pid_t pid;                  /* process ID */
	char completed;             /* true if process has completed */
	char stopped;               /* true if process has stopped */
	int status;                 /* reported status value */
} process;

/* A job is a pipeline of processes.  */
typedef struct job
{
	struct job *next;           /* next active job */
	char *command;              /* command line, used for messages */
	process
	       	*first_process;     /* list of processes in this job */
	pid_t pgid;                 /* process group ID */
	char notified;              /* true if user told about stopped job */
	struct termios tmodes;      /* saved terminal modes */
	int stdin, stdout, stderr;  /* standard i/o channels */
} job;

/* On free de manière récursive tous les process  */
void free_process(process* p);

/* On free le job et tous ses process */
void free_job(job* j);

/* Find the active job with the indicated pgid.  */
job *find_job (pid_t pgid);

/* Permet l'initialisation d'un process */
void initialize_process(process* p,char* commande,int cpt_espace,ssize_t taille);

/* Permet l'initialisation d'un job */
void initialize_job(job* job,char* commande,process* p,int stdin,int stdout);

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (job *j);

/* Return true if all processes in the job have completed.  */
int job_is_completed (job *j);

void init_shell ();

void launch_process (process *p, pid_t pgid,int infile, int outfile, int errfile,int foreground);

void launch_job (job *j, int foreground);

void put_job_in_foreground (job *j, int cont);

void put_job_in_background (job *j, int cont);

int mark_process_status (pid_t pid, int status);

void update_status (void);

void wait_for_job (job *j);

void format_job_info (job *j, const char *status);

void do_job_notification (void);

void mark_job_as_running (job *j);

void continue_job (job *j, int foreground);

/* Permet d'avoir la taille du plus grand mot de la commande pour le malloc */
int taille_max(char* commande,ssize_t taille);

/* Permet l'allocation d'un process */
void alloc_process(process* p,char* commande,ssize_t taille);

/* Permet de tester s'il y a redirection et si oui laquelle */
void test_chevron(char** argv,int taille,int* t_entree,int* t_sortie,int* t_sortie_append);

/* Permet de parse la commande en fonction des pipes */
int coupe_pipe(char* commande,char **commandes);

/* Permet de compter les espaces d'un commande */
int cpt_espacef(char* commande,ssize_t taille);

/* Permet d'initialiser n process en cas de pipe */
void initialize_n_process(process* first,char** commandes,int cpt_commandes);

/* Permet de savoir s'il faut mettre en background ou foreground */
int is_background(char * commande,int taille);