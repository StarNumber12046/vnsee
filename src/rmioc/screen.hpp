#ifndef RMIOC_SCREEN_HPP
#define RMIOC_SCREEN_HPP

#include "mxcfb.hpp"
#include <cstdint>
#include <linux/fb.h>

namespace rmioc
{

/**
 * Information and resources for using the device screen.
 */
class screen
{
public:
    screen();
    ~screen();

    /**
     * Update a partial region of the screen.
     *
     * @param x Left bound of the region to update (in pixels).
     * @param y Top bound of the region to update (in pixels).
     * @param w Width of the region to update (in pixels).
     * @param h Height of the region to update (in pixels).
     * @param mode Update mode to use (default GC16).
     * @param wait True to block until the update is complete.
     */
    void update(
        int x, int y, int w, int h,
        mxcfb::waveform_modes mode = mxcfb::waveform_modes::gc16,
        bool wait = false);

    /**
     * Perform a full update of the screen.
     *
     * @param mode Update mode to use (default GC16).
     * @param wait True to block until the update is complete.
     */
    void update(
        mxcfb::waveform_modes mode = mxcfb::waveform_modes::gc16,
        bool wait = true);

    /**
     * Access the screen data buffer.
     *
     * This is a contiguous row-major-order array of pixels.
     *
     * Each row of this array contains `get_xres_memory()` pixels, which
     * may be greater than the actual `get_xres()` number of visible pixels.
     *
     * There are `get_yres_memory()` rows, which again may be greater than
     * the actual `get_yres()` number of visible rows.
     *
     * Each pixel is represented by `get_bits_per_pixel()` bits split among
     * red, blue and green components as specified by the
     * `get_{red|green|blue}_{offset|length}()` values.
     */
    std::uint8_t* get_data();

    std::uint32_t get_xres() const;
    std::uint32_t get_xres_memory() const;

    std::uint32_t get_yres() const;
    std::uint32_t get_yres_memory() const;

    std::uint32_t get_bits_per_pixel() const;

    std::uint32_t get_red_offset() const;
    std::uint32_t get_red_length() const;
    std::uint32_t get_red_max() const;

    std::uint32_t get_green_offset() const;
    std::uint32_t get_green_length() const;
    std::uint32_t get_green_max() const;

    std::uint32_t get_blue_offset() const;
    std::uint32_t get_blue_length() const;
    std::uint32_t get_blue_max() const;

private:
    /** File descriptor for the device framebuffer. */
    int framebuf_fd;

    /** Variable screen information from the device framebuffer. */
    fb_var_screeninfo framebuf_varinfo{};

    /** Fixed screen information from the device framebuffer. */
    fb_fix_screeninfo framebuf_fixinfo{};

    /** Pointer to the memory-mapped framebuffer. */
    std::uint8_t* framebuf_ptr = nullptr;

    /**
     * Send an update object to the framebuffer driver.
     *
     * @param update Update object to send.
     * @param wait True to wait until update is complete.
     */
    void send_update(mxcfb::update_data& update, bool wait);

    /** Next value to be used as an update marker. */
    std::uint32_t next_update_marker = 1;

    /** Maximum value to use for update markers. */
    static constexpr std::uint32_t max_update_marker = 255;
}; // class screen

} // namespace rmioc

#endif // RMIOC_SCREEN_HPP
