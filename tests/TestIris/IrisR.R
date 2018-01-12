library(MASS)


data = iris

good_res = c(data[,5])
color = good_res

for(i in 1:length(color))
{
  if(color[i] == 1)
    color[i] = "red"
  if(color[i] == 2)
    color[i] = "blue"
  if(color[i] == 3)
    color[i] = "green"
}

plot(data[,-5], col = color)


countMax = 0
for(dd in 1:50000)
{
  km = kmeans(x = data[,-5], centers = 3, nstart = 1, iter.max = 1)
  
  count = 0

  for(i in 1:length(good_res))
  {
    if(good_res[i] == km$cluster[i])
    {
      count = count + 1
    }
    else
    {
      
    }
  }
  if(count > countMax)
  {
    countMax = count
    print(countMax/length(good_res))
  }
  #print(count/length(good_res))
}
  print(countMax/length(good_res))