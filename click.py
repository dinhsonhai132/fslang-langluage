import pygame
import random


def click():

    pygame.init()


    width = 650
    height = 650
    screen = pygame.display.set_mode((width, height))


    RED = (255, 0, 0)
    BLUE = (0, 0, 255)
    WHITE = (255, 255, 255)
    PURPLE = (138, 43, 226)


    def make_text(text, font_size, color, bool):
        font = pygame.font.Font(None, font_size)
        if bool == 0:
            Text = font.render(text, True, color)
            return Text
        else:
            return 0


    size = 50
    x = random.randint(0, width - size)
    y = random.randint(0, height - size)
    clock = pygame.time.Clock()

    real_point = 0
    point = 0

    start_time = pygame.time.get_ticks()
    last_reset_time = start_time


    run = True

    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                    run = False


        mouse = pygame.mouse.get_pos()

        random_rect = pygame.Rect(x, y, size, size)
        player = pygame.Rect(mouse[0], mouse[1], 1, 1)
        

        screen.fill(WHITE)

        
        pygame.draw.rect(screen, (0, 255, 23), random_rect)
        current_time = pygame.time.get_ticks()


        if player.colliderect(random_rect) and pygame.mouse.get_pressed()[0]:
            x = random.randint(0, width - size)
            y = random.randint(0, height - size)
            last_reset_time = current_time
            real_point += 5
            point += 5

        elif (current_time - last_reset_time) / 1000 >= 1:
            x = random.randint(0, width - size)
            y = random.randint(0, height - size)
            last_reset_time = current_time
            real_point -= 1
            point -= 1

        
        if point >= 50:
            point = 0
            size -= 5
        

        if size <= 10:
            size = 50


        text = make_text("score: {}".format(real_point), 40, (0, 255, 255), 0)
        screen.blit(text, (0, 0))


        clock.tick(65)
        get_fps = str(clock.get_fps())
        fps = get_fps[:4]


        if float(get_fps) < 50 and float(get_fps) > 30:
            fps_check = make_text("fps {} normal".format(fps), 40, RED, 0)
        elif float(get_fps) < 30:
            fps_check = make_text("fps {} low".format(fps), 40, PURPLE, 0)
        else:
            fps_check = make_text("fps {} high".format(fps), 40, BLUE, 0)

        screen.blit(fps_check, (0, 40))

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                run = False

        pygame.display.flip()
        clock.tick(60)

    pygame.quit()