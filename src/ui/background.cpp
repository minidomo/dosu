#include "./background.h"

#include <File.hpp>
#include <Image.hpp>
#include <ImageTexture.hpp>
#include <Ref.hpp>
#include <ResourceLoader.hpp>

#include "singleton/game.h"

void Background::_register_methods() {
    register_method("_ready", &Background::_ready);

    register_property<Background, float>("dim", &Background::dim, .3f);
}

void Background::_init() { dim = .3f; }

void Background::_ready() {
    background_path = Game::get_singleton(this)->get_default_background_path();

    background_image = get_node<TextureRect>("Image");
    dimmer = get_node<ColorRect>("Dim");

    update_image();
    dimmer->set_frame_color(Color(0, 0, 0, dim));
}

void Background::set_background_path(String background_path) {
    if (background_path != this->background_path) {
        this->background_path = background_path;
        update_image();
    }
}

void Background::set_dim(float dim) {
    if (dim != this->dim) {
        this->dim = dim;
        dimmer->set_frame_color(Color(0, 0, 0, dim));
    }
}

String Background::get_background_path() { return background_path; }

float Background::get_dim() { return dim; }

void Background::update_image() {
    File *file = File::_new();
    dev_assert(file->open(background_path, File::READ) == Error::OK);

    auto bytes = file->get_buffer(file->get_len());
    file->close();

    Image *image = Image::_new();
    dev_assert(image->load_jpg_from_buffer(bytes) == Error::OK);
    Ref<Image> ref_image(image);

    auto texture = ImageTexture::_new();
    texture->create_from_image(ref_image);
    Ref<Texture> ref_texture(texture);

    background_image->set_texture(ref_texture);
}

void Background::update_background(Background *background, Beatmap *beatmap) {
    String bg_filename = beatmap->get_background_filename();
    String bg_path;

    if (bg_filename.empty()) {
        bg_path =
            Game::get_singleton(background)->get_default_background_path();
    } else {
        bg_path = Game::get_singleton(background)->get_songs_dir_path() + "/" +
                  beatmap->get_beatmap_set_id() + "/" + bg_filename;
    }

    background->set_background_path(bg_path);
}