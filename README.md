<img width="910" height="458" alt="image" src="https://github.com/user-attachments/assets/8e620031-aa10-49cd-a0f4-a61adce0c6af" />

# Playful Linked Robots

This project involves two small devices that can send simple messages to each other over wifi. It has a cute UI, running on an ESP32-C3 Mini. It has 6 customizable input buttons and displays messages on the screen while playing audio effects via a speaker. 

The reason I decided to make this project is twofold. Professionally, I really wanted to learn hardware but I haven't done a hardware project before. So, I started with the concept of the Spotify Display hardware project but I wanted to make it more in line with my interests and needs and that brings me to my second reason being personal. I am going away this summer and my sister (6) has been really worried by this. We've grown up really close and I took care of her a lot given our age difference and this is the first time we'll be away from each other. So these devices are supposed to be a low tech communication tool she can use as well over the summer. This dictated some of the design decisions i.e. the monkey casing because those are my sisters favourite animals. But technically, creating messaging devices that function almost like a mini flip phone is really cool to me and I think understanding these basics through this project allows for more exploration and functionality in the future. 

A specific list of hardware can be found in the BOM at the bottom of this page but to summarize, each devices has:
- an ESP32-C3 Mini
- a 1.8" ST7735 TFT display
- 6 buttons
- a MAX98357A audio amplifier + speaker 
- a PCF8575 I/O expander for reading the buttons
wired as seen in the image below. 



<img width="601" height="445" alt="image" src="https://github.com/user-attachments/assets/12728324-c0a2-4a0c-b3b3-aad83ac7e6ef" />



The project is housed in 3D printed cases. The cases are decorated with a couple features but things like eyes and the colours will be painted on post production. 




<img width="843" height="840" alt="image" src="https://github.com/user-attachments/assets/95485104-16c6-47a4-bcc4-587e8b19279f" />




The project works by, when pressing a button on device A, a message is sent to a tiny server which device B can access. Device B is checking the server and when it gets a message it displays it on screen.  The same thing happens in reverse when device B sends a message. The functionality is made in the firmware documents. there is one for each robot but the only difference is the robots id when making a log in the server. You can customize messages by changing the section under the messages comment. To run this project on your own, you should make your own server on Supabase to house your messages and add your respective API keys and wifis



## Bill of Materials 

| Name | Purpose | Quantity | Total Cost (USD) | Link | Distributor |
|------|--------|----------|------------------|------|-------------|
| 3Dâ€‘Printed Monkey Body | This contains all of the electronics | 1 | 10 | https://github.com/GaiyatriJain/Playful-Linked-Robots | I can get it from my local library, it's 15 cents per gram, and i estimated 50-100 grams (avg 10 dollars) |
| Silicone wires | Connects all the boards | 1 | 5.59 | https://www.amazon.ca/silicone-18AWG-16AWG-14AWG-12AWG/dp/B0CY518WT7/ref=sr_1_23?crid=2MKA0QIB561ID&dib=eyJ2IjoiMSJ9.DB6nc8KN3ZnmJ9Vk5W_aOHECAQA0YRy1dZMnbtDboGzj5X4bmxpl5CrYNa91Lg5lqoDZtarVRSj5JU1lzCtwBCadJ3LwAug-rS9bvZl9DMD-FLJtZx4IGBJqmGlAjTlOBEU_PRqKPcwH4igb4b__0D6TavGgQiIbq09u9AUP2wF0Mg_S8vIj5AKwPkDDq1MW8bvpqGiUj0ngI0e0oEOfV71uOIUpOUpsRuUoRGQSnVfkb1QxEwQuE9KVCqoSRqkZuGxtAKwg1VNhNc9bdv3k-wHLrRzgALcfbU_vmZbG9do.1RMCw6eLk70MkYGO_PSCy2EdVYV-jpeb2IWEXhOpFMo&dib_tag=se&keywords=Silicone+Wire&qid=1775093838&s=industrial&sprefix=silicone+wire%2Cindustrial%2C119&sr=1-23 | Amazon |
| Keyboard Switches | To control the input buttons | 1 | 7.63 | https://www.amazon.ca/Replacement-Keyboard-Mechanical-Feelings-NewGiant/dp/B0866GCR6S/ref=asc_df_B0866GCR6S?tag=bingshopdesk-20&linkCode=df0&hvadid=80264547654465&hvnetw=o&hvqmt=e&hvbmt=be&hvdev=c&hvlocint=&hvlocphy=5254&hvtargid=pla-4583864015378407&psc=1&msclkid=d2a3b13cf12d1a276e78d8f136abe6f0 | Amazon |
| 28â€¯mm 8Î© 1W Speaker | Allows us to play audio from the amplifier board | 1 | 9.99 | https://www.amazon.ca/uxcell-Internal-Speaker-Magnet-Loudspeaker/dp/B07YYVZ6R6/ref=sr_1_2_sspa?crid=2LV8FFBQZ6YC4&dib=eyJ2IjoiMSJ9.QRpzMvepvtSzYnxSi7B18NHWZipt7D1JoZDwoio0hi0BX4zy-4LV0yMA1fJxCpuk220YoHBxKUX8RbysxFr-SQs3a5Gyc3h4EJJ8ppNmnj9KaIkysJk5UYPcIVo-wtdmrFAb0PAK4d576RMTN7CccvFRTjf70QxhKWA1_6HGHxaYs92kj0inpUaf5XzQKiD0ZArRovmAa1D2PkzplR9AoXfVPp4nr5nhCOz4sEVF6RoqPKb4I4Sqvv0vaDAn0pqckYzOAk9Xm6DUacueAs_jEmUVSuTV_YVVdKx2Ny5U40g.9ZAqbXEq1tCk8qrNXmW5QFQzh7xqb8eQFQpaY8cCGSk&dib_tag=se&keywords=28+mm+8%CE%A9+1W+Speaker&qid=1775093430&sprefix=28+mm+8%CF%89+1w+speaker+%2Caps%2C76&sr=8-2-spons&sp_csd=d2lkZ2V0TmFtZT1zcF9hdGY&psc=1 | Amazon |
| MAX98357A I2S Input Auditory Amplifier Board | Allows us to play sounds | 1 | 10.79 | https://www.amazon.ca/MAX98357A-Amplifier-Electronic-Enthusiasts-Engineers/dp/B0GDGLX9WD/ref=asc_df_B0GDGLX9WD?tag=bingshopdesk-20&linkCode=df0&hvadid=80608154716258&hvnetw=o&hvqmt=e&hvbmt=be&hvdev=c&hvlocint=&hvlocphy=5254&hvtargid=pla-4584207634626937&psc=1&msclkid=cf4c57d2a72918c556b73e1f612ba9f1 | Amazon |
| PCF8575 I/O Expander | adds 16 inputs using only 2 pins which allows for all the buttons and electronics | 1 | 3.71 | https://www.amazon.ca/Parallel-Expander-PCF8575-Board-Shifter/dp/B08T7XXHT4/ref=asc_df_B08T7XXHT4?tag=bingshopdesk-20&linkCode=df0&hvadid=79920950061564&hvnetw=o&hvqmt=e&hvbmt=be&hvdev=c&hvlocint=&hvlocphy=5254&hvtargid=pla-4583520413333525&psc=1&msclkid=6afe8f6e6e8c13fed12de6a3ab385806 | Amazon |
| TFT Screen Module | The screen that displays the UI, drawings, animations etc. | 1 | 13.63 | https://www.amazon.ca/1-8inch-Resolution-Interface-Full-Color-Controller/dp/B083NYBN4Q/ref=asc_df_B083NYBN4Q?tag=bingshopdesk-20&linkCode=df0&hvadid=80745476895770&hvnetw=o&hvqmt=e&hvbmt=be&hvdev=c&hvlocint=&hvlocphy=5254&hvtargid=pla-4584345044715580&psc=1 | Amazon |
| ESP32 Board | This is the microcontroller for the project so like the projects brain | 1 | 13.99 | https://www.amazon.ca/ESP-WROOM-32-NodeMCU-Bluetooth-Development-Microcontroller/dp/B0CHBMFJBQ/ref=asc_df_B0CHBMFJBQ?tag=bingshopdesk-20&linkCode=df0&hvadid=79920843185609&hvnetw=o&hvqmt=e&hvbmt=be&hvdev=c&hvlocint=&hvlocphy=5254&hvtargid=pla-4583520407436381&th=1 | Amazon |
