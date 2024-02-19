#!/bin/sh
# run in root directory of project
sed -i '/\[submodule "libft"\]/,/active = true/d' .git/config
git submodule deinit -f -- lib/libft
git rm lib/libft
rm -rf .git/modules/lib/libft
git submodule add https://github.com/LeonDavidZipp/libft.git lib/libft
git submodule update --init --recursive
git add .gitmodules lib/libft
git commit -m "Update libft submodule"
git push