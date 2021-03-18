/*****************************************************************************
* | File      	:   EPD_5in83_test.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper test demo
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
#include "EPD_5in83.h"

int EPD_5in83_test(void)
{
    printf("EPD_5IN83_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_5IN83_Init();
    EPD_5IN83_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_5IN83_WIDTH % 8 == 0)? (EPD_5IN83_WIDTH / 8 ): (EPD_5IN83_WIDTH / 8 + 1)) * EPD_5IN83_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize/4)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_5IN83_WIDTH, EPD_5IN83_HEIGHT/4, 0, WHITE);


/****************************************************************
		Since the RAM of STM32 is insufficient and buffers one 
		frame of data, it is convenient to refresh one frame of data.
		
		Divided into 4 pieces here
*****************************************************************/
#if 0   // show image for array   
    printf("show image for array\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
		
    Paint_DrawBitMap_Block(gImage_5in83, 1);
		EPD_5IN83_Display_OneQuarter(BlackImage, 1);

		Paint_DrawBitMap_Block(gImage_5in83, 2);
		EPD_5IN83_Display_OneQuarter(BlackImage, 2);
		
		Paint_DrawBitMap_Block(gImage_5in83, 3);
		EPD_5IN83_Display_OneQuarter(BlackImage, 3);
		
		Paint_DrawBitMap_Block(gImage_5in83, 4);
		EPD_5IN83_Display_OneQuarter(BlackImage, 4);
		
		EPD_5IN83_UpdateDisplay();
    DEV_Delay_ms(500);

#else 
		/*show sd card pic*/
		printf("show sd card pic\r\n");
    //1.Initialize the SD card
    SDCard_Init();
		SDCard_ReadBMP_SetBlock(4);
		
    //2.Create a new image cache named IMAGE_BW and fill it with white
    Paint_Clear(WHITE);
		
		//3.Read BMP pictures into RAM
		SDCard_ReadBMP_Block("5in83.bmp", 1);
		EPD_5IN83_Display_OneQuarter(BlackImage, 1);
		
		SDCard_ReadBMP_Block("5in83.bmp", 2);
		EPD_5IN83_Display_OneQuarter(BlackImage, 2);
		
		SDCard_ReadBMP_Block("5in83.bmp", 3);
		EPD_5IN83_Display_OneQuarter(BlackImage, 3);
		
		SDCard_ReadBMP_Block("5in83.bmp", 4);
		EPD_5IN83_Display_OneQuarter(BlackImage, 4);
		
    //4.Refresh the picture in RAM to e-Paper
		printf("EPD_5IN83_Display\r\n");
    EPD_5IN83_UpdateDisplay();
    DEV_Delay_ms(2000);
		
#endif

#if 1   // Drawing on the image
    //1.Select Image
    printf("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    
    Paint_DrawLine(20, 70, 70, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 100, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 100, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(130, 0, " ÄãºÃabc", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20, "Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
		EPD_5IN83_Display_OneQuarter(BlackImage, 1);
		Paint_Clear(WHITE);
		
		Paint_DrawPoint(10, 50, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 60, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 70, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
		Paint_DrawCircle(45, 65, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 65, 20, BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
		Paint_DrawLine(85, 65, 125, 65, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 45, 105, 85, BLACK, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
		EPD_5IN83_Display_OneQuarter(BlackImage, 2);
		Paint_Clear(WHITE);
		
		EPD_5IN83_Display_OneQuarter(BlackImage, 3);
		Paint_Clear(WHITE);
		
		EPD_5IN83_Display_OneQuarter(BlackImage, 4);

		EPD_5IN83_UpdateDisplay();
		
    printf("EPD_Display\r\n");
    DEV_Delay_ms(2000);

#endif

    printf("Clear...\r\n");
    EPD_5IN83_Clear();

    printf("Goto Sleep...\r\n");
    EPD_5IN83_Sleep();
    free(BlackImage);
    BlackImage = NULL;

    // close 5V
    printf("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
    
    return 0;
}

