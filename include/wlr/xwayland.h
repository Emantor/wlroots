#ifndef WLR_XWAYLAND_H
#define WLR_XWAYLAND_H

#include <time.h>
#include <stdbool.h>
#include <wlr/types/wlr_compositor.h>
#include <xcb/xcb.h>

struct wlr_xwm;

struct wlr_xwayland {
	pid_t pid;
	int display;
	int x_fd[2], wl_fd[2], wm_fd[2];
	struct wl_client *client;
	struct wl_display *wl_display;
	struct wlr_compositor *compositor;
	time_t server_start;

	struct wl_event_source *sigusr1_source;
	struct wl_listener destroy_listener;
	struct wlr_xwm *xwm;
	struct wl_list displayable_surfaces;

	struct {
		struct wl_signal new_surface;
	} events;

	void *data;
};

struct wlr_xwayland_surface {
	xcb_window_t window_id;
	uint32_t surface_id;
	struct wl_list link;

	struct wlr_surface *surface;
	struct wl_listener surface_destroy_listener;
	int16_t x, y;
	uint16_t width, height;
	bool override_redirect;

	char *title;
	char *class;
	char *instance;
	struct wlr_xwayland_surface *parent;
	list_t *state; // list of xcb_atom_t
	pid_t pid;

	xcb_atom_t *window_type;
	size_t window_type_len;

	xcb_atom_t *protocols;
	size_t protocols_len;

	struct {
		struct wl_signal destroy;

		struct wl_signal request_configure;

		struct wl_signal set_title;
		struct wl_signal set_class;
		struct wl_signal set_parent;
		struct wl_signal set_state;
		struct wl_signal set_pid;
		struct wl_signal set_window_type;
	} events;

	void *data;
};

struct wlr_xwayland_surface_configure_event {
	struct wlr_xwayland_surface *surface;
	int16_t x, y;
	uint16_t width, height;
};

void wlr_xwayland_destroy(struct wlr_xwayland *wlr_xwayland);
struct wlr_xwayland *wlr_xwayland_create(struct wl_display *wl_display,
	struct wlr_compositor *compositor);
void wlr_xwayland_surface_activate(struct wlr_xwayland *wlr_xwayland,
	struct wlr_xwayland_surface *surface);
void wlr_xwayland_surface_configure(struct wlr_xwayland *wlr_xwayland,
	struct wlr_xwayland_surface *surface, int16_t x, int16_t y,
	uint16_t width, uint16_t height);
void wlr_xwayland_surface_close(struct wlr_xwayland *wlr_xwayland,
	struct wlr_xwayland_surface *surface);

#endif
