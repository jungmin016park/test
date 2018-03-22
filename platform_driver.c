/* device tree
  test_gpio {
    compatible = "test,control-gpio";
    test,reset-gpio = <>;
  }
*/

#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/of_gpio.h>
#include <linux/test_gpio.h>
#include <linux/miscdevice.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/compat.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

struct test_struct {
  int reset_gpio;
};
struct test_struct test_db;

static long test_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
  if ((_IOC_TYPE(cmd) != TEST_IOCTL_MAGIC) ||
      (_IOC_NR(cmd) == 0) || 
      (_IOC_NR(cmd) > TEST_IOCTL_MAXNR))    { 
      pr_err("%s : bad cmd type cmd : %d\n", __func__, cmd);  
      return -EFAULT;   
  }
  
  switch (cmd) {
    case IOCTL_SET_GPIO :
      gpio_set_value(test_db.reset_gpio, 1);
      break;
    case IOCTL_GET_GPIO :
      val = gpio_get_value(test_db.reset_gpio);
      put_user(val, (int __user*)arg;
    default :
      pr_err("%s : ioctl bad command\n", __func__);         
      return -EFAULT;
  }
  
  return 0;
}

static const struct file_operations test_fops = { 
	.owner      = THIS_MODULE,  
	.unlocked_ioctl = test_ioctl,
};

static struct miscdevice test_miscdev = {  
  .minor = MISC_DYNAMIC_MINOR,    
  .name  = "test-gpio",   
  .fops  = &test_fops,     
  .mode  = 0666,
};

static int mobis_control_probe(struct platform_device *pdev) { 
  struct device_node *node = pdev->dev.of_node;
  struct device *dev = &pdev->dev;
  int ret=0;
  
  dev_set_drvdata(dev, &test_db); 
  
  test_db.set_gpio = of_get_named_gpio(node, "test,reset-gpio", 0);
  
  if (test_db.reset_gpio == -EPROBE_DEFER) {
    pr_err("%s : gpio EPROBE_DEFER\n", __func__);
    return -EPROBE_DEFER;
  }
 
  if (!gpio_is_valid(test_db.reset_gpio)) {
    pr_err("%s : gpio is not valid\n", __func__);
    return -ENODEV;
  }
 
  ret = misc_register(&test_miscdev);
  if(ret < 0) {
    pr_err("%s : error misc register, ret : %d\n", __func__, ret);   
    return ret;
  }
  
  return ret;
}

static const struct of_device_id test_of_match[] = {    
  {.compatible = "test,control-gpio" },   
  {},
};
MODULE_DEVICE_TABLE(of, test_of_match);

static struct platform_driver test_driver = {   
  .probe = test_probe,   
  .driver = {      
  .name = "test",      
  .of_match_table = test_of_match,    
  .owner = THIS_MODULE,   
  },
};

static int __init test_init(void) {
  return platform_driver_register(&test_driver);
}
static void __exit test_exit(void) {
  platform_driver_unregister(&test_driver);
}

module_init(test_init);
module_exit(test_exit);

MODULE_DESCRIPTION("test");
MODULE_AUTHOR("Jungmin Park");
MODULE_LICENSE("GPL v2");
