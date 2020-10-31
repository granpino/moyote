##
#!/usr/bin/python
# Raspberry pi mqtt 
# This script reads the temperature data from data.txt and will run moyote.py
# The installation of mosquitto, mosquitto-clients, and paho-mqtt
# is required
import sys, pygame
import time
from pygame.locals import *
import datetime
import subprocess 
  
pygame.init()
size = width, height = 640, 430

#screen = pygame.display.set_mode(size)
screen = pygame.display.set_mode(size, pygame.FULLSCREEN)
cyan = 50, 255, 255
black = 0, 0, 0
white = 255, 255, 255
red = 255, 0, 0
gray = 45, 45, 45

subprocess.call("sudo python moyote.py &", shell=True)

def read_data():
    global line1
    global line2
    file1 = open("data.txt","r+")
    lines = file1.readline().split("| ")
    if len(lines)==1:
        line1 = "00"
        line2 = "00"
    else:
        line1 = lines[0]
        line2 = lines[1]


def refresh_screen():
    screen.fill(gray)
    current_time = datetime.datetime.now().strftime('%I :%M')
    font1=pygame.font.Font('Digital-7.ttf',150)
    font2=pygame.font.Font(None, 40)
    time_label = font1.render(current_time, 1, (red))
    time2_label = font1.render("88 :88", 1, (gray))
    pygame.draw.rect(screen, black, (20, 20, 600, 390),0) 
    screen.blit(time2_label,(140,120))
    screen.blit(time_label,(140,120))
    label1 = font2.render(line1 + "'F", 1, (cyan))
    label2 = font2.render(line2 + "%", 1, (cyan))
    screen.blit(label1, (200, 270))
    screen.blit(label2, (400, 270))
    pygame.display.flip()


def main():
    while True:
        read_data()
        for x in range(10): # reads the data every 5 sec
            for event in pygame.event.get():
                if event.type == pygame.MOUSEBUTTONDOWN:
                    click_pos = pygame.mouse.get_pos() # click at center of screen
                    print click_pos
                    if 270 <= click_pos[0] <= 400 and 120 <= click_pos[1] <250:
                        subprocess.call("sudo pkill -f moyote.py", shell=True)
                        sys.exit()
                if event.type == KEYDOWN:
                    if event.key == K_ESCAPE: # press ESC to exit
                        subprocess.call("sudo pkill -f moyote.py", shell=True)
                        sys.exit()
            time.sleep(0.5)
            refresh_screen()


#update_weather()
main()
