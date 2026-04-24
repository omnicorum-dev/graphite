//
// Created by Nico Russo on 4/19/26.
//

#include <graphite.h>

int main() {
    const Graphite::Canvas canvas(800, 600);

    canvas.fill(Graphite::Colors::LightGrey);

    canvas.writeOmniStringBaseline("A B C D E F G H I J K L M \n"
                                   "N O P Q R S T U V W X Y Z \n"
                                   "a b c d e f g h i j k l m \n"
                                   "n o p q r s t u v w x y z \n"
                                   "0 1 2 3 4 5 6 7 8 9 \n"
                                   ". , ! ? : ; * ( ) [ ] { } \n"
                                   "\' \" ` ~ _ $", 10, 80, 6);

    if (!canvas.saveToJPG("../testing/new_output.jpg")) {
        omni::LOG_ERROR("Failed to save to JPEG");
        return 1;
    }

    return 0;
}