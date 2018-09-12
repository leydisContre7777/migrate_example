<?php

function migrate_example_migrate_api() {

	$api = array(
		'api' => 2,
		'groups' => array(
			'cgtt_product' => array(
				'title' => t('CGTT products'),
			),
		),
		'migrations' => array(
			'Subject' => array(
				'class_name' => 'SubjectTermMigration',
				'group_name' => 'cgtt_product',
			),
			'Requirements' => array(
				'class_name' => 'RequerimentTermMigration',
				'group_name' => 'cgtt_product',
			),
			'Companies' => array(
				'class_name' => 'CompanyTermMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'Consejeria' => array(
				'class_name' => 'ConsejeriaTermMigration',
				'group_name' => 'cgtt_product',
			),
			'Projects' => array(
				'class_name' => 'ProjectMigration',
				'group_name' => 'cgtt_product',
				'dependencies' => array(
					'Subject',
					'Requirements',
					'Companies',
					'Consejeria'
				),
			),
			'PublicVersion' => array(
				'class_name' => 'PublicVersionMigration',
				'group_name' => 'cgtt_product',
				'dependencies' => array('Projects'),
			),
			'PrivateVersion' => array(
				'class_name' => 'PrivateVersionMigration',
				'group_name' => 'cgtt_product',
				'dependencies' => array('Projects'),
			),
			'Tag' => array(
				'class_name' => 'TagTermMigration',
				'group_name' => 'cgtt_product',
			),
			'NewSubject' => array(
				'class_name' => 'NewSubjectMigration',
				'group_name' => 'cgtt_product',
				'event_log' => ''
			),
			'News' => array(
				'class_name' => 'NewsMigration',
				'group_name' => 'cgtt_product',
			),
			'Documentation' => array(
				'class_name' => 'DocumentationMigration',
				'group_name' => 'cgtt_product',
			),
			'Tipology' => array(
				'class_name' => 'TipologyMigration',
				'group_name' => 'cgtt_product',
			),
		),
	);

	return $api;


}


