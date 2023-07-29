SUMBER = Dir.glob("src/*.c")
TUJUAN = "gadiren.exe"
INCLUDE_JALUR = %w( include ../service/include )
# BEFRAYPROFIL = "BefrayMinim"
# BEFRAYLIBRARY = File.join("D:/Singgah/mruby/build", File.join(BEFRAYPROFIL, "lib"))
# BEFRAYOPTS = "-L #{BEFRAYLIBRARY} -lmruby -liphlpapi -lws2_32"
BEFRAYOPTS = "-Wl,-subsystem,windows"
OBJFILE = SUMBER.collect do |s|
  obj = s.sub(/\.c$/, '.o')
  obj.sub(/^src/, 'build')
end
GETSRC = proc do |task_name|
  name = task_name.sub(/^build/, 'src')
  name.sub(/.o$/, '.c')
end

rule '.o' => GETSRC do |t|
  include_opt = INCLUDE_JALUR.collect { |i| "-I " + i } .join(' ')
  sh "gcc #{include_opt} -o #{t.name} -c #{t.prerequisites.first}"
end

file TUJUAN => OBJFILE do
  # sh 'windres -i src/alamat.rc -O coff -o build/alamat.res'
  # sh "gcc #{OBJFILE.join(" ")} build/alamat.res #{BEFRAYOPTS} -o #{TUJUAN}"
  sh "gcc #{OBJFILE.join(" ")} #{BEFRAYOPTS} -o #{TUJUAN}"
end

task :reset do 
  sh 'del ' + File.join(ENV["APPDATA"], "Gadiren", "sarang.bin").gsub(/\//, "\\")
end

task :clean do 
  sh 'rmdir /S /Q build'
  sh 'del ' + TUJUAN
end

Dir.mkdir('build') unless Dir.exist?('build')
task default: TUJUAN