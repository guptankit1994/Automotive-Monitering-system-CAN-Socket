#include"can_header.h"
#include<time.h>
#include <stdio.h>
#include "curl/curl.h"
#define ON 1               
#define OFF 0

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Stri    ng start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds) ; 
} 

 void c_curl(long speed)
{
 printf("curl speed %ld",speed);
  
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http/:Example");
    /* Now specify the POST data */
    char strSpeed[100];
    sprintf(strSpeed, "%s%ld","speed=",speed);
    char url[1000] = "&right_indicator=1&left_indicator=0";
    strcat(strSpeed, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strSpeed);
    printf("string is %s",strSpeed);
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

}
 
int main()
{
  

    int  nbytes,c;
    long speed,current_speed;
    int turn_status[2];
    struct sockaddr_can addr;
    struct ifreq ifr;
    socklen_t len = sizeof(addr);
    struct canfd_frame frame;
   
    struct can_filter rfilter[1];    // filter structure added    
    const char *ifname = "vcan0";
    
 if((c = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)                         
        {
		      perror("client: Error while opening socket");
		      return -1;
	}

    strcpy(ifr.ifr_name, ifname);
    ioctl(c, SIOCGIFINDEX, &ifr);
	
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    
   


    if(bind(c, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
	{
		perror("client:Error in socket bind");
		return -2;
	}



       rfilter[0].can_id   = 580;
   //  rfilter[1].can_id   = 392;
       rfilter[0].can_mask = CAN_SFF_MASK;
    // rfilter[1].can_mask = CAN_SFF_MASK;

    setsockopt(c, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));   // FILTER 


long prev_value = 0;
//for(int i=0;i<500;i++)
for(;;)
{
   
     nbytes = read(c, &frame, sizeof(struct canfd_frame));

    if (nbytes < 0) 
     {
            perror("client: can raw socket read");
	   
            return 1;
     }               
    
    
   
   //if(frame.data[0]>90)
   // {
     // continue;
    //}
  // else
    //{
          speed = frame.data[3] << 8;  // parsing speed
	  speed += frame.data[3 + 1];
	  speed = speed / 100;         // speed in kilometers
	  current_speed = speed * 0.6213751; // mph
  		
	  if(frame.data[0] & 1)     // LEFT TURN STATUS
          {
          turn_status[0] = ON;
          } 
         else 
          {
          turn_status[0] = OFF;
          }
        if(frame.data[0] & 2)     // RIGHT TURN STATUS 
          {
           turn_status[1] = ON;
          } 
        else 
         {
           turn_status[1] = OFF;
         }

if(((current_speed%5) == 0)){
    
  
 if( prev_value != current_speed){

       printf("\nCAN RECIEVER: SPEED= %ld , Left=%d , Right=%d\n",current_speed,turn_status[0],turn_status[1]);

        c_curl(current_speed);
    }
        prev_value = current_speed;
}
    
        //delay(1);
}
    
    return 0;

 }
