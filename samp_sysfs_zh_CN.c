/* 
 * sysfs.example.c - sample code for Documentation/filesystems/sysfs.txt
 * 
 * Copyright (C) 2013  Bill Wang(Liang) 
 * (bill.liangwlw@gmail.com,https://github.com/billwangwl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kobject.h>
#include <linux/device.h>

static struct kobject *hwobj=NULL; 

ssize_t show_node(struct device *dev, struct device_attribute *att, char *buf)
{
	pr_info("%s called\n",__func__);
	return scnprintf(buf, sizeof("show_node"), "%s", "show_node");
}

ssize_t store_node(struct device *dev, struct device_attribute *att, const char *buf,
		size_t count)
{
	pr_info("%s store value %s\n",__func__, buf);
	return sizeof(buf);
}


static DEVICE_ATTR(write_node, S_IWUSR | S_IRUGO, NULL, store_node);
static DEVICE_ATTR(read_node, S_IWUSR | S_IRUGO, show_node, NULL);

static int __init samp_sysfs_init(void)
{
	int ret = 0;
	pr_info("%s module init\n", __func__);
	hwobj = kobject_create_and_add("hwobj", NULL);
	if (IS_ERR(hwobj)){
		ret = PTR_ERR(hwobj);
		pr_err("object alloc failed\n");
		goto exit_del_kobject;
	}

	ret = sysfs_create_file(hwobj, &dev_attr_read_node.attr);
	if (ret){
		pr_err("create read node failed %d\n", ret);
		goto exit_del_sysfs_read;
	}

	ret = sysfs_create_file(hwobj, &dev_attr_write_node.attr);
	if (ret){
		pr_err("create write node failed %d\n", ret);
		goto exit_del_sysfs_write_node;
	}
	return ret;

exit_del_sysfs_write_node:
	sysfs_remove_file(hwobj, &dev_attr_write_node.attr);
exit_del_sysfs_read:
	sysfs_remove_file(hwobj, &dev_attr_read_node.attr);
exit_del_kobject:
	kobject_del(hwobj);
	pr_info("%s return %d \n", __func__, ret);

	return ret;
}
static void __exit samp_sysfs_cleanup(void)
{
	sysfs_remove_file(hwobj, &dev_attr_write_node.attr);
	sysfs_remove_file(hwobj, &dev_attr_read_node.attr);
	kobject_del(hwobj);
	printk(KERN_INFO "hell_cleanup\n");
}

module_init(samp_sysfs_init);
module_exit(samp_sysfs_cleanup);

MODULE_AUTHOR("Bill Wang(Liang) <bill.liangwlw@gmail.com>");
MODULE_DESCRIPTION("sample code for Documentation/filesystems/sysfs.txt");
MODULE_LICENSE("GPL");

