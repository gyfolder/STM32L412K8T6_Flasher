/*****************************************************************************
* | File      	:   EPD_5in83bc_test.c
* | Author      :   Waveshare team
* | Function    :   5.83inch B&C e-paper test demo
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-13
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_Test.h"
#include "EPD_5in83bc.h"

int EPD_5in83bc_test(void)
{
    printf("EPD_5IN83BC_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_5IN83BC_Init();
    EPD_5IN83BC_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RYImage;
    UWORD Imagesize = ((EPD_5IN83BC_WIDTH % 8 == 0)? (EPD_5IN83BC_WIDTH / 8 ): (EPD_5IN83BC_WIDTH / 8 + 1)) * EPD_5IN83BC_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize / 8)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    if((RYImage = (UBYTE *)malloc(Imagesize / 8)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        return -1;
    }
    printf("NewImage:BlackImage and RYImage\r\n");
    Paint_NewImage(BlackImage, EPD_5IN83BC_WIDTH, EPD_5IN83BC_HEIGHT / 8, 0, WHITE);
    Paint_NewImage(RYImage, EPD_5IN83BC_WIDTH, EPD_5IN83BC_HEIGHT / 8, 0, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RYImage);
    Paint_Clear(WHITE);

#if 0   // show image for array    
    printf("show image for array\r\n");
		for(int i=1; i<9; i++){
			Paint_SelectImage(BlackImage);
			Paint_DrawBitMap_Block(gImage_5in83bc_b, i);
		
			Paint_SelectImage(RYImage);
			Paint_DrawBitMap_Block(gImage_5in83bc_ry, i);
			EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, i);
		}
		EPD_5IN83BC_UpdateDisplay();
    DEV_Delay_ms(2000);
#else 
		/*show sd card pic*/
    //Initialize the SD card
    SDCard_Init();
		SDCard_ReadBMP_SetBlock(8);
		SDCard_ReadBMP_DataInversion(1);
		/*Red and black two pictures, one picture is 
		divided into 8 pieces, a total of 16 pieces*/
		
		for(int i=1; i<9; i++){
			Paint_SelectImage(BlackImage);
			Paint_Clear(WHITE);
			SDCard_ReadBMP_Block("5in83b-b.bmp", i);
			
			Paint_SelectImage(RYImage);
			Paint_Clear(WHITE);
			SDCard_ReadBMP_Block("5in83b-r.bmp", i);
			
			EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, i);
		}
		EPD_5IN83BC_UpdateDisplay();
    DEV_Delay_ms(3000);
		
#endif

#if 1   // Drawing on the image
    /*Horizontal screen*/
    //1.Draw black image
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
		Paint_DrawPoint(10, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 20, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 30, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
		Paint_DrawString_EN(50, 0, "waveshare", &Font16, BLACK, WHITE);    
    Paint_DrawString_CN(220, 0, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
		Paint_SelectImage(RYImage);
		Paint_Clear(WHITE);
		Paint_DrawPoint(10, 40, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
		Paint_DrawString_CN(50, 30,"ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_EN(120, 38, "hello world", &Font12, WHITE, BLACK);
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 1);
		
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
		Paint_DrawLine(20, 1, 70, 50, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 1, 20, 50, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);      
    Paint_DrawRectangle(20, 1, 70, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 1, 130, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
		Paint_SelectImage(RYImage);
		Paint_Clear(WHITE);
		Paint_DrawLine(80, 1, 130, 50, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(130, 1, 80, 50, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);   
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 2);
		
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
    Paint_DrawCircle(210, 21, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(140, 21, 180, 21, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
		Paint_DrawLine(210, 1, 210, 41, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
		Paint_DrawNum(10, 1, 987654321, &Font16, WHITE, BLACK);
		Paint_SelectImage(RYImage);
		Paint_Clear(WHITE);
		Paint_DrawCircle(160, 21, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawLine(190, 21, 230, 21, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(160, 1, 160, 41, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED); 
		Paint_DrawNum(10, 30, 123456789, &Font12, BLACK, WHITE);
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 3);
		
		
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
		
		Paint_SelectImage(RYImage);
		Paint_Clear(WHITE);
		
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 4);
		
		Paint_SelectImage(BlackImage);
		Paint_Clear(WHITE);
		
		Paint_SelectImage(RYImage);
		Paint_Clear(WHITE);
		
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 5);
		
		Paint_SelectImage(BlackImage);

		Paint_SelectImage(RYImage);

		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 6);
		
		Paint_SelectImage(BlackImage);

		Paint_SelectImage(RYImage);
	
		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 7);
		
		Paint_SelectImage(BlackImage);

		Paint_SelectImage(RYImage);

		EPD_5IN83BC_Display_OneEighth(BlackImage, RYImage, 8);
    
    printf("EPD_Display\r\n");
		
		EPD_5IN83BC_UpdateDisplay();
    //EPD_5IN83BC_DisplayHalfScreen(BlackImage, RYImage);
    DEV_Delay_ms(2000);
		
#endif

    printf("Clear...\r\n");
    EPD_5IN83BC_Clear();

    printf("Goto Sleep...\r\n");
    EPD_5IN83BC_Sleep();
    free(BlackImage);
    free(RYImage);
    BlackImage = NULL;
    RYImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

