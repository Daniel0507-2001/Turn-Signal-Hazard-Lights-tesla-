#include "main.h"
#define DEBOUNCE_TIME 300



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t left_toggles = 0;
uint32_t right_toggles = 0,hazard_toggles=0;
uint32_t last_debounce_time_left = 0;
uint32_t last_debounce_time_right = 0,last_debounce_time_hazard = 0;
uint32_t counter_right=0, counter_left=0,counter_hazard=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
  uint32_t current_time = HAL_GetTick();

  if (GPIO_Pin == S1_Pin){
	  if (current_time - last_debounce_time_left >= DEBOUNCE_TIME) {

          if (current_time - last_debounce_time_left > 1000 && counter_left < 2) {
              counter_left = 0;
          }


		  counter_left++;
		  last_debounce_time_left = current_time;



		  if(counter_left>0){
		 			  right_toggles=0;
		 		  }

		  if(counter_left==1){

        	  HAL_UART_Transmit(&huart2, "S1\r\n", 4, 10);
        	  left_toggles = 6;
          }
          else if(counter_left==2){
        	  HAL_UART_Transmit(&huart2, "S1_toggles\r\n",12, 10);
        	  left_toggles = 0xEEEEEEE;



          }
          else if (counter_left>=3){
        	  HAL_UART_Transmit(&huart2, "S1_off\r\n",8, 10);
        	  counter_left=0;
        	  left_toggles = 0;



          }


     }


  } else if (GPIO_Pin == S2_Pin){
	  if (current_time - last_debounce_time_right >= DEBOUNCE_TIME) {


          if(current_time - last_debounce_time_right > 1000 && counter_right<2){
        	  counter_right=0;
          }
	       counter_right++;
	       last_debounce_time_right = current_time;

			  if(counter_right>0){
			 			  left_toggles=0;
			 		  }



	      if(counter_right==1){

	          HAL_UART_Transmit(&huart2, "S2\r\n",4, 10);
	          right_toggles = 6;



	          }

	          else if(counter_right==2){

	        	  HAL_UART_Transmit(&huart2, "S2_toggles\r\n", 12, 10);
	              right_toggles = 0xEEEEEEE;



	          }
	          else if(counter_right>=3){

	        	  HAL_UART_Transmit(&huart2, "S2_off\r\n", 8, 10);
	              right_toggles = 0;
	         	  counter_right = 0;


	          }


	          }

	  }else if(GPIO_Pin==S3_Pin){

		  if (current_time - last_debounce_time_right >= DEBOUNCE_TIME) {


		       counter_hazard++;
		       last_debounce_time_right = current_time;

				  if(counter_hazard>0){
					  left_toggles = 0;
					  right_toggles = 0;
				  }


		      if(counter_hazard==1){

		          HAL_UART_Transmit(&huart2, "S3_on\r\n",7, 10);
		          right_toggles = 10;
		          left_toggles = 10;

		          hazard_toggles=1;



		          }
//
		          else if(counter_hazard>=2){

		        	  HAL_UART_Transmit(&huart2, "S3_off\r\n", 8, 10);
		              right_toggles = 0;
		              left_toggles = 0;
		         	  counter_hazard = 0;
		         	  hazard_toggles=0;



		          }


		          }




	  }

  }


  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void heartbeat (void){

	static uint32_t heartbeat_tick = 0;
	if(heartbeat_tick  < HAL_GetTick() ){
		heartbeat_tick = HAL_GetTick() + 500;
		HAL_GPIO_TogglePin(D1_GPIO_Port, D1_Pin);
	}
}
/* USER CODE END 0 */

void turn_signal_left (void){
	static uint32_t tunr_togle_tick = 0;
	if(tunr_togle_tick  < HAL_GetTick() ){

		if(left_toggles> 0){
			tunr_togle_tick = HAL_GetTick() + 300;
			HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
			left_toggles--;
		} else{

			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		}

	}
}



void turn_signal_right (void){

	static uint32_t tunr_togle_tick = 0;
	if(tunr_togle_tick  < HAL_GetTick() ){
		if(right_toggles> 0){
			tunr_togle_tick = HAL_GetTick() + 300;
			HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
			right_toggles--;

		} else{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);

		}

	}
}




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  HAL_Init();

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  heartbeat();
	  turn_signal_left();
	  turn_signal_right();

	  // hazard signal
	  if(counter_hazard==1){
		  HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
		  HAL_Delay(300);
		  HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 0);
		  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 0);
		  HAL_Delay(300);


	  }




	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
