
require 'fminer'
puts 'Loaded'

fm = Fminer::Fminer.new()
puts 'Created'

fm.AddCompound("C1CCCCC1",1)
fm.AddCompound("C1CCCCC1",2)
fm.AddCompound("C1CCCCC1",3)
fm.AddCompound("C1CCCCC1",4)
fm.AddCompound("C1CCCCC1",5)
puts 'Comps'

fm.AddActivity(true, 1)
fm.AddActivity(true, 2)
fm.AddActivity(true, 3)
fm.AddActivity(true, 4)
fm.AddActivity(false, 5)
puts 'Acts'

print fm.GetNoCompounds()  
puts " compounds"

(0 .. fm.GetNoRootNodes()-1).each do |j|
    result = fm.MineRoot(j)
    
    (0 .. result.size-1).each do |i|
        puts i
    end
end
