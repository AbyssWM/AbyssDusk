globber = run_command('sh', '-c', 'find . -name "*.cpp" | sort', check : true) src = globber.stdout().strip().split('\n')

                                                                                         executable('abyssdusk', src,
                                                                                                    dependencies :
                                                                                                    [
                                                                                                        dependency('wayland-client'),
                                                                                                        dependency('wayland-cursor'),
                                                                                                        dependency('abyssparse'),
                                                                                                        dependency('abyssutils', version : '>= 0.2.3'),
                                                                                                        dependency('threads'),
                                                                                                    ],
                                                                                                    install : true, )
