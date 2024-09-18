#include "../../../recasting.h"

#include <time.h>

#define TARGET_FPS 60
#define FRAME_TARGET_TIME (1000 / TARGET_FPS)

void loop_fun(void* param)
{
	t_cub* cube = (t_cub*)param;
	static clock_t previous_frame_time = 0;
	clock_t current_time = clock();
	double delta_time = (current_time - previous_frame_time) / (double)CLOCKS_PER_SEC;

	if (delta_time >= 1.0 / TARGET_FPS)
	{
		update_player(cube);
		ft_draw_sky_floor(cube);
		cube->threads[0].id = 0;
		cube->threads[1].id = 1;
		pthread_create(&cube->threads[0].thread, NULL, &draw_lines_3d, (void *)cube);
		pthread_create(&cube->threads[1].thread, NULL, &draw_lines_3d_1, (void *)cube);
		pthread_join(cube->threads[0].thread, NULL);
		pthread_join(cube->threads[1].thread, NULL);

		draw_per(cube);
		update_y_press(cube);
		draw_gun_right_click(cube);
		previous_frame_time = current_time;
	}
	clock_t frame_time = clock() - current_time;
	if (frame_time < FRAME_TARGET_TIME)
	{
		struct timespec rem, req = {0, (FRAME_TARGET_TIME - frame_time) * 1000000};
		nanosleep(&req, &rem);
	}
	// Mouse handling
	int32_t xpos, ypos;
	mlx_get_mouse_pos(cube->mlx, &xpos, &ypos);

	if (xpos != WIDTH / 2 && ypos != HEIGHT / 2 && (cube->player->start == 0 || cube->player->start == 1))
	{
		mlx_set_mouse_pos(cube->mlx, WIDTH / 2, HEIGHT / 2);
		cube->player->start += 1;
	}
	else if (cube->player->shift == 1)
		handle_mouse(cube);
	else
		mlx_set_cursor_mode(cube->mlx, MLX_MOUSE_NORMAL);
}
// // end hooks
